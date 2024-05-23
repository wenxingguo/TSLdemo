#ifndef TSLPARSER_H
#define TSLPARSER_H

#include "json.hpp"
#include "typedef.h"
#include <fstream>
#include <unordered_set>

static std::string find_or_empty(const nlohmann::json& item, const std::string& key)
{
    auto iter = item.find(key);
    if (iter != item.cend()) {
        return iter.value().get<std::string>();
    }
    return "";
}

// 用以去除重复定义
static bool test_insert_Key_in_Set(std::unordered_set<std::string>& set, const std::string& key)
{
    auto p = set.insert(key);
    return p.second;
}

/****************************************************************************************
 * 增加返回信息只需要在下列代码添加相应的变量
 * enum，struct 需要定义到全局
 ***************************************************************************************/
#define GET_COMMAN_FILED(item)                                  \
    std::string declare;                                        \
    std::string identifier = find_or_empty(item, "identifier"); \
    std::string desc = find_or_empty(item, "desc");             \
    std::string name = find_or_empty(item, "name");             \
    const nlohmann::json* dataType;                             \
    if (identifier.empty()) {                                   \
        dataType = &item;                                       \
    } else {                                                    \
        dataType = &item.at("dataType");                        \
    }                                                           \
    std::string type = find_or_empty(*dataType, "type");        \
    std::string accessMode = find_or_empty(*dataType, "accessMode")

#define RETURN_ITEM_INFO \
    return { declare, identifier, desc, name, type, accessMode }

class TSLParser {
    // typedef std::vector<std::vector<std::string>> EquipmentContentItem_Type;

private:
    std::unordered_set<std::string> definite_set;
    nlohmann::json TSL_Json_Handle;
    EquipmentContentItem_Type Contents;
    EquipmentDeclareItem_Type Declares;

public:
    TSLParser(const std::string& TSLFile_)
    {
        if (!TSLFile_.empty()) {
            std::ifstream f(TSLFile_);
            f >> TSL_Json_Handle;
            // parse();
        }
    }

    const nlohmann::json& getTSLHandle() const
    {
        return TSL_Json_Handle;
    }

    EquipmentContentItem_Type getEquipmentContents() const
    {
        // std::vector(vector&& other)
        return std::move(Contents);
    }

    EquipmentDeclareItem_Type getEquipmentDeclares() const
    {
        return std::move(Declares);
    }

    std::vector<std::string> ProcessProfile() const
    {
        const nlohmann::json& profile = TSL_Json_Handle.at("profile");
        std::string productKey = find_or_empty(profile, "productKey");
        std::string version = find_or_empty(profile, "version");
        return { productKey, version };
    }

    void parse()
    {
        // 处理Properties
        nlohmann::json::const_iterator properties_iter = TSL_Json_Handle.find("properties");
        const nlohmann::json& properties_json = properties_iter.value();
        for (int j = 0; j < properties_json.size(); ++j) {
            const nlohmann::json& properties_json_item = properties_json.at(j);
            ProcessSingleProperty(properties_json_item);
        }
        // 处理Services
        nlohmann::json::const_iterator services_iter = TSL_Json_Handle.find("services");
        const nlohmann::json& services_json = services_iter.value();
        for (int j = 0; j < services_json.size(); ++j) {
            const nlohmann::json& services_json_item = services_json.at(j);
            ProcessSingleService(services_json_item);
        }
    }

    virtual ~TSLParser() { }

private:
    std::vector<std::string> podTypeDeclare(const nlohmann::json& item) // int bool double float text date
    {
        GET_COMMAN_FILED(item);
        if (type == "int" || type == "bool")
            declare = "int";
        if (type == "text" || type == "date")
            declare = "std::string";
        if (type == "double" || type == "float")
            declare = type;
        RETURN_ITEM_INFO;
    }

    std::vector<std::string> enumTypeDeclare(const nlohmann::json& item)
    {
        GET_COMMAN_FILED(item);
        std::vector<std::vector<std::string>> enum_items;
        // {declare(结尾包含的类型 _enum), desc, name} -> {enum_item, value}

        declare += identifier + "_enum";

        const nlohmann::json& specs = dataType->at("specs");
        std::map<int, std::string> enum_map;
        for (nlohmann::json::const_iterator enum_item = specs.cbegin(); enum_item != specs.cend(); ++enum_item) {
            enum_map.insert({ std::stoi(enum_item.key()), enum_item.value().get<std::string>() });
        }
        for (auto i : enum_map) {
            enum_items.push_back({ i.second, std::to_string(i.first) });
        }

        if (test_insert_Key_in_Set(definite_set, declare)) {
            Declares.push_back({ { declare, desc, name }, enum_items });
        }
        RETURN_ITEM_INFO;
    }

    std::vector<std::string> allTypeDeclare(const nlohmann::json& item, const std::string& identifier_in = "")
    {
        GET_COMMAN_FILED(item);
        assert(type != "");

        //处理outputdata 因为其identifier都为result，会冲突
        if (identifier == "result") {
            identifier = identifier_in + "_" + identifier;
        }

        if (type == "int" || type == "text" || type == "double" || type == "float" || type == "date" || type == "bool")
            return podTypeDeclare(item);
        if (type == "enum")
            return enumTypeDeclare(item);

        // aggregate Type
        const nlohmann::json& specs = dataType->at("specs");
        if (type == "struct") {
            if (identifier.empty()) { // 处理array里面的匿名struct
                declare = identifier_in + "_struct";
            } else {
                declare = identifier + "_struct";
            }
            std::vector<std::vector<std::string>> struct_items;
            // 处理正常的struct
            // {declare(结尾包含的类型 _struct), desc, name, scope(inputData/outputdata/Equipment)} -> {declare, identifier, type, desc, name}
            assert(declare != "");
            for (int i = 0; i < specs.size(); ++i) {
                const nlohmann::json& struct_json_item = specs.at(i);
                std::vector<std::string> result = allTypeDeclare(struct_json_item);
                // return { declare, identifier, desc, name, type, accessMode }
                struct_items.push_back({ result[0], result[1], result[5], result[2], result[3] });
            }

            if (test_insert_Key_in_Set(definite_set, declare)) {
                Declares.push_back({ { declare, desc, name }, struct_items });
            }
        }
        if (type == "array") {
            const nlohmann::json& array_json_item = specs.at("item");
            std::string array_size = specs.at("size").get<std::string>();
            std::string array_item_type = array_json_item.at("type").get<std::string>();
            std::string array_declare_shuffle;
            if (array_item_type == "struct") {
                array_declare_shuffle = "_array_item";
            }

            auto result = allTypeDeclare(array_json_item, identifier + array_declare_shuffle);
            declare = "std::vector<" + result[0] + ">"; // array使用std::vector<>
        }

        RETURN_ITEM_INFO;
    }

    void ProcessSingleService(const nlohmann::json& services_json_item)
    {
        std::string callType = find_or_empty(services_json_item, "callType");
        std::string serviceIdentifier = find_or_empty(services_json_item, "identifier");
        std::string desc = find_or_empty(services_json_item, "desc");
        std::string name = find_or_empty(services_json_item, "desc");
        const nlohmann::json& inputData = services_json_item.at("inputData");
        const nlohmann::json& outputData = services_json_item.at("outputData");

        std::string outputData_var_declaration;

        if (outputData.empty()) {
            outputData_var_declaration = "void";
        } else {
            auto outputData_info = allTypeDeclare(outputData.at(0), serviceIdentifier);
            outputData_var_declaration = outputData_info[0];
        }
        //{service, type_declare, identifier, inputData_declaration 1, inputData_identifier 2, ... ...}
        std::vector<std::string> service_item = { "service", desc, name, callType, outputData_var_declaration, serviceIdentifier };

        for (int index = 0; index < inputData.size(); ++index) {
            auto inputData_info = allTypeDeclare(inputData.at(index));

            service_item.push_back(inputData_info[0]);
            service_item.push_back(inputData_info[1]);
        }

        assert(outputData.size() < 2);
        Contents.push_back(service_item);
    }

    void ProcessSingleProperty(const nlohmann::json& property_json_item)
    {
        auto result = allTypeDeclare(property_json_item);
        // return { declare, identifier, desc, name, type, accessMode }
        Contents.push_back({ "property", result[0], result[1], result[5], result[4], result[2], result[3] });
    }
};

#endif /* TSLPARSER_H */
