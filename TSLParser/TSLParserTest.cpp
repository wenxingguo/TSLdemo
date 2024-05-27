#include "TSLParser.hpp"
#include <iostream>

void formatClassName(std::string& s)
{
    for (auto& i : s) {
        if (i == '.')
            i = '_';
    }
}

std::string genEquipmentRegistration(const std::string& plugin_name, const EquipmentContentItem_Type& plugin)
{
    std::string result;
    result += ("registration::class_<" + plugin_name + ">(" + "\"" + plugin_name + "\")\n");
    for (auto item : plugin) {
        if (item[0] == "property") {
            // {property, declare, identifier, accessMode, type, desc, name}
            result += ".property( \"" + item[2] + "\", " + "&" + plugin_name + "::" + item[2] + ")\n";
        }
    }
    for (auto item : plugin) {
        if (item[0] == "service") {
            //{service, desc, name, callType, type_declare, identifier, inputData_declaration 1, inputData_identifier 2, ... ...}
            result += ".method(\"" + item[5] + "\"," + "&" + plugin_name + "::" + item[5] + ")\n";
        }
    }
    result += ";\n";
    return result;
}

std::string genGlobalDefinition(const EquipmentDeclareItem_Type& declare_item_info)
{
    std::string result;
    for (const auto& declare_item : declare_item_info) {
        const std::vector<std::string>& declare_info = declare_item.first;
        std::string declare = declare_info[0];
        auto declare_content_item = declare_item.second;
        if (declare.ends_with("_struct")) {
            // {declare(结尾包含的类型 _struct), desc, name, scope(inputData/outputdata/Equipment)} -> {declare, identifier, type, desc, name}
            result += ("struct " + declare + "{ // " + declare_info[1] + " " + declare_info[2] + "\n");
            for (int i = 0; i < declare_content_item.size(); ++i) {
                std::vector<std::string> struct_item = declare_content_item[i];
                result += (struct_item[0] + " " + struct_item[1] + "; //" + struct_item[3] + " " + struct_item[4] + "\n\n");
            }
            result += "};\n\n";
        }
        if (declare.ends_with("_enum")) {
            // {declare(结尾包含的类型 _enum), desc, name, scope(inputData/outputdata/Equipment)} -> {enum_item, value}
            result += ("enum class " + declare + "{ //" + declare_info[1] + " " + declare_info[2] + "\n");
            for (int i = 0; i < declare_content_item.size(); ++i) {
                std::vector<std::string> enum_item = declare_content_item[i];
                result += (enum_item[0] + "=" + enum_item[1]);
                if (i != (declare_content_item.size() - 1))
                    result += ",";
                result += "\n";
            }

            result += "};\n\n";
        }
    }

    return result;
}

std::string genGlobalRegistration(const EquipmentDeclareItem_Type& declare_item_info)
{
    std::string result;
    for (const auto& declare_item : declare_item_info) {
        std::string item_result = "registration::";
        const std::vector<std::string>& declare_info = declare_item.first;
        std::string declare = declare_info[0];
        auto declare_content_item = declare_item.second;
        if (declare.ends_with("_struct")) {
            // {declare(结尾包含的类型 _struct), desc, name} -> {declare, identifier, type, desc, name}
            item_result += ("class_<" + declare + ">(" + "\"" + declare + "\")\n");
            for (int i = 0; i < declare_content_item.size(); ++i) {
                std::vector<std::string> struct_item = declare_content_item[i];
                item_result += (".property(\"" + struct_item[1] + "\", " + "&" + declare + "::" + struct_item[1] + ")\n");
            }
            item_result += ";\n\n";
        }
        if (declare.ends_with("_enum")) {
            // {declare(结尾包含的类型 _enum), desc, name} -> {enum_item, value}
            item_result += ("enumeration<" + declare + ">(" + "\"" + declare + "\")\n(\n");
            for (int i = 0; i < declare_content_item.size(); ++i) {
                std::vector<std::string> enum_item = declare_content_item[i];
                item_result += ("value( \"" + enum_item[0] + "\"," + declare + "::" + enum_item[0] + ")");
                // result += (enum_item[0] + "=" + enum_item[1]);
                if (i != (declare_content_item.size() - 1))
                    item_result += ",";
                item_result += "\n";
            }

            item_result += "\n);\n\n";
        }
        result += item_result;
    }

    return result;
}

std::string genEquipmentDefinition(const EquipmentContentItem_Type& plugin_class_info)
{
    std::string result;
    for (auto item : plugin_class_info) {
        if (item[0] == "service") {
            //{service, desc, name, callType, type_declare, identifier, inputData_declaration 1, inputData_identifier 2, ... ...}
            result += (item[4] + " " + item[5] + "(");
            for (int i = 6; i < item.size(); i += 2) {
                result += (item[i] + " " + item[i + 1]);
                if (i != (item.size() - 2))
                    result += ", ";
            }
            result += (") // " + item[1] + " " + item[2] + " " + item[3] + "\n{\n // code \n}\n");
        }
    }

    result += "public:\n";
    for (auto item : plugin_class_info) {
        // {property, declare, identifier, accessMode, type, desc, name}
        if (item[0] == "property")
            result += (item[1] + " " + item[2] + "; // " + item[5] + " " + item[6] + "\n\n");
    }
    // result += "RTTR_ENABLE(Equipment)\n";

    // result += "};\n\n";

    return result;
}

void test(const std::string& TSLFile)
{
    TSLParser parser(TSLFile);
    parser.parse();
    const nlohmann::json& Handle = parser.getTSLHandle();

    auto profile_info = parser.ProcessProfile();
    formatClassName(profile_info[0]);
    formatClassName(profile_info[1]);

    std::string Equipment_name = profile_info[0] + profile_info[1];
    EquipmentContentItem_Type Contents = parser.getEquipmentContents();
    EquipmentDeclareItem_Type Declares = parser.getEquipmentDeclares();

    std::cout << "/**************************************************" << std::endl;
    std::cout << "* 该文件由 TSLParserTest生成" << std::endl;
    std::cout << "**************************************************/" << std::endl;

    // 头文件
    std::cout << "#include <string>" << std::endl;
    std::cout << "#include <vector>" << std::endl;

    std::cout << "\n";

    // 生成全局定义
    std::cout << genGlobalDefinition(Declares);

    std::cout << "class " << (Equipment_name) << "{ \n"
              << std::endl;
    std::cout << "public:" << std::endl;
    // std::cout << "bool init(const std::string& msg) override {}" << std::endl;
    // std::cout << "bool connect() override {}" << std::endl;
    // std::cout << "bool disconnect() override {}" << std::endl;

    std::cout << "// 服务" << std::endl;
    std::cout << genEquipmentDefinition(Contents) << std::endl;

    std::cout << "};\n\n"
              << std::endl;

    //注册
    // std::cout << "RTTR_PLUGIN_REGISTRATION {\n using namespace rttr;" << std::endl;
    // std::cout << genEquipmentRegistration(Equipment_name, Contents) << std::endl;
    // std::cout << genGlobalRegistration(Declares) << std::endl;

    // std::cout << "}" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "no json file specific!!" << std::endl;
        exit(-1);
    }

    std::string TSLFile = argv[1];
    test(TSLFile);
    return 0;
}
