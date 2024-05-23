#include "Equipment.h"

std::unordered_map<std::string, std::shared_ptr<EquipmentContentItem_Type>> Equipment::Contents_Map;
std::unordered_map<std::string, std::shared_ptr<EquipmentDeclareItem_Type>> Equipment::Declares_Map;
