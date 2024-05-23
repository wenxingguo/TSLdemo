#ifndef EQUIPMENT_TYPEDEF
#define EQUIPMENT_TYPEDEF

#include <string>
#include <vector>

/**************************************
 * {property, declare, identifier, accessMode, type, desc, name}
 * {service, outputData_declare, identifier, inputData_declaration, inputData_identifier, desc, name}
 * {event} 暂不支持
 **************************************/
typedef std::vector<std::vector<std::string>> EquipmentContentItem_Type;

/**************************************
 * {declare(结尾包含的类型 _struct), desc, name, scope(inputData/outputdata/Equipment)} -> {declare, identifier, type, desc, name}
 * {declare(结尾包含的类型 _enum), desc, name, scope(inputData/outputdata/Equipment)} -> {enum_item, value}
 *************************************/
typedef std::vector<std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>> EquipmentDeclareItem_Type;

#endif /* ifndef EQUIPMENT_TYPEDEF */
