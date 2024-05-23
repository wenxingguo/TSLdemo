#include "HttpEquipment.h"

std::unordered_map<std::string, std::shared_ptr<HttpEquipmentUrls_Type>> HttpEquipment::Urls_Map;

HttpEquipment::HttpEquipment(const std::string& TSLFile_, const std::string& IP_,
    ReturnMsgHandle S_retmsg_handle, ReturnMsgHandle P_retmsg_handle)
    : Equipment(TSLFile_, EquipmentType::EQUIPMENT_HTTP)
    , IP(IP_)
    , Contents_ptr(nullptr)
    , Declares_ptr(nullptr)
    , Urls_ptr(nullptr)
    , service_ret_mag_handle(S_retmsg_handle)
    , property_ret_mag_handle(P_retmsg_handle)
{
    parseTSLFile();
}

std::string HttpEquipment::invokeService(const std::string& service_identifier, const std::vector<std::string>& argsvs)
{

    // argsvs = {"service", identifier, url, key1=value1, key2=value2, .....}

    // 构造url
    std::string service_url = "http://" + IP + getUrlbyIdentifier(service_identifier);
    if (!argsvs.empty()) {
        // 存在输入参数
        service_url += "?";
        for (int i = 0; i < argsvs.size(); ++i) {
            service_url += argsvs[i];
            if (i != (argsvs.size() - 1))
                service_url += "&";
        }
    }

    return HttpClient::connect_once(service_url, service_ret_mag_handle);
}

std::string HttpEquipment::getProPerty(const std::string& property_identifier)
{
    std::string property_url = "http://" + IP + getUrlbyIdentifier(property_identifier);
    return HttpClient::connect_once(property_url, property_ret_mag_handle);
}

void HttpEquipment::parseTSLFile()
{
    TSLParser parser(TSLFile);
    // 处理profile
    //
    auto profile_info = parser.ProcessProfile();
    EquipmentName = profile_info[0] + profile_info[1];

    bool parsed_before = false;

    auto content_map_iter = Contents_Map.find(EquipmentName);
    if (content_map_iter == Contents_Map.cend()) {
        Contents_ptr.reset(new EquipmentContentItem_Type);
        *Contents_ptr = parser.getEquipmentContents();
        Contents_Map.insert({ EquipmentName, Contents_ptr });
    } else {
        Contents_ptr = content_map_iter->second;
        parsed_before = true;
    }

    auto declare_map_iter = Declares_Map.find(EquipmentName);
    if (declare_map_iter == Declares_Map.cend()) {
        assert(parsed_before == false);
        Declares_ptr.reset(new EquipmentDeclareItem_Type);
        *Declares_ptr = parser.getEquipmentDeclares();
        Declares_Map.insert({ EquipmentName, Declares_ptr });
    } else {
        assert(parsed_before == true);
        Declares_ptr = declare_map_iter->second;
    }

    // 解析url
    if (parsed_before == true) {
        auto urls_map_iter = Urls_Map.find(EquipmentName);
        assert(urls_map_iter != Urls_Map.cend());
        Urls_ptr = urls_map_iter->second;
        // 在map中寻找
    } else {
        // 解析并加入map中
        Urls_ptr.reset(new HttpEquipmentUrls_Type);
        const nlohmann::json& Handle = parser.getTSLHandle();
        const nlohmann::json& customizeConfig = Handle.at("extendConfig").at("customizeConfig");
        // 仅处理services
        const nlohmann::json& service_extend_conf = customizeConfig.at("services");
        for (const auto& item : service_extend_conf) {
            // {"service", "identifier", "url"}
            Urls_ptr->push_back({ "service", item.at("identifier").get<std::string>(), item.at("content").get<std::string>() });
        }
        // 处理properties
        const nlohmann::json& property_extend_conf = customizeConfig.at("properties");
        for (const auto& item : property_extend_conf) {
            // {"service", "identifier", "url"}
            Urls_ptr->push_back({ "property", item.at("identifier").get<std::string>(), item.at("content").get<std::string>() });
        }
        Urls_Map.insert({ EquipmentName, Urls_ptr });
    }
}

void HttpEquipment::setPropertyRetmsg_Handle(ReturnMsgHandle handle)
{
    property_ret_mag_handle = handle;
}

void HttpEquipment::setServieRetmsg_Handle(ReturnMsgHandle handle)
{
    service_ret_mag_handle = handle;
}

std::string HttpEquipment::getUrlbyIdentifier(const std::string& Identifier)
{
    assert(Urls_ptr);
    auto cmp_fn = [Identifier](const std::vector<std::string>& url_item) {
        return url_item[1] == Identifier;
    };
    HttpEquipmentUrls_Type::iterator iter = std::find_if(Urls_ptr->begin(), Urls_ptr->end(), cmp_fn);
    assert(iter != Urls_ptr->end());
    return (*iter)[2];
}
