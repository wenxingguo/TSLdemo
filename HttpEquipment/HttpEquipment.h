#ifndef HTTPEQUIPMENT_H
#define HTTPEQUIPMENT_H

#include "../Equipment.h"
#include "../TSLParser.hpp"
#include "HttpClient.hpp"

typedef std::vector<std::vector<std::string>> HttpEquipmentUrls_Type; //{service identifier urls }

class HttpEquipment : public Equipment {
private:
    std::string IP;
    std::shared_ptr<EquipmentContentItem_Type> Contents_ptr;
    std::shared_ptr<EquipmentDeclareItem_Type> Declares_ptr;
    std::shared_ptr<HttpEquipmentUrls_Type> Urls_ptr;

    static std::unordered_map<std::string, std::shared_ptr<HttpEquipmentUrls_Type>> Urls_Map;

public:
    HttpEquipment(const std::string& TSLFile_, const std::string& IP_);

    std::string invokeService(const std::string& service_identifier, const std::vector<std::string>& argsvs);

    std::string getProPerty(const std::string& property_identifier);

    virtual ~HttpEquipment() { }

private:
    void parseTSLFile();

    std::string getUrlbyIdentifier(const std::string& Identifier);
};

#endif /* HTTPEQUIPMENT_H */
