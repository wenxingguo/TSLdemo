#ifndef EQUIPMENT_H
#define EQUIPMENT_H

//#include "TSLParser/TSLParser.hpp"
#include "typedef.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**************************************
 * {property, declare, identifier, accessMode, type, desc, name}
 * {service, outputData_declare, identifier, inputData_declaration, inputData_identifier, desc, name}
 * {event} 暂不支持
 **************************************/
// typedef std::vector<std::vector<std::string>> EquipmentContentItem_Type;

/**************************************
 * {declare(结尾包含的类型 _struct), desc, name, scope(inputData/outputdata/Equipment)} -> {declare, identifier, type, desc, name}
 * {declare(结尾包含的类型 _enum), desc, name, scope(inputData/outputdata/Equipment)} -> {enum_item, value}
 *************************************/
// typedef std::vector<std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>> EquipmentDeclareItem_Type;

enum class EquipmentType {
    EQUIPMENT_NONE = -1,
    EQUIPMENT_CUSTOM = 0,
    EQUIPMENT_HTTP = 1,
};

class Equipment {

protected:
    std::string EquipmentName;
    std::string TSLFile;
    EquipmentType EqType;

    static std::unordered_map<std::string, std::shared_ptr<EquipmentContentItem_Type>> Contents_Map;
    static std::unordered_map<std::string, std::shared_ptr<EquipmentDeclareItem_Type>> Declares_Map;

public:
    Equipment(const std::string& TSLFile_, EquipmentType EqType_ = EquipmentType::EQUIPMENT_NONE)
        : TSLFile(TSLFile_)
        , EqType(EqType_)
    {
    }
    Equipment()
        : EqType(EquipmentType::EQUIPMENT_NONE)
    {
    }

    virtual void parseTSLFile() = 0;

    virtual std::string getProPerty(const std::string& property_identifier) = 0;

    virtual std::string invokeService(const std::string& service_identifier, const std::vector<std::string>& argvs) = 0;
};
#endif
