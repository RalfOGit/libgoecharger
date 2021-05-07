#include <GoEChargerAPI.hpp>
#include <HttpClient.hpp>


GoEChargerAPI::GoEChargerAPI(void) :
    map(NULL) {}

GoEChargerAPI::~GoEChargerAPI(void) {
    if (map != NULL) {
        json_value* json = (json_value*)&map->getJson();
        json_value_free(json);
        map = NULL;
    }
}

bool GoEChargerAPI::refreshMap(const std::string charger_url) {

    // send http status request
    HttpClient http_client;
    std::string response;
    std::string content;
    http_client.sendHttpGetRequest(charger_url, response, content);

    // parse json content and allocate new GoEChargerDataMap to access its content
    json_value* json = json_parse(content.c_str(), content.length());
    if (json != NULL) {
        map = new GoEChargerDataMap(*json);
        printf("%s\n", map->toString().c_str());
        return true;
    }
    return false;
}

uint8_t GoEChargerAPI::getUint8(const char* const key) {
    if (map != NULL) {
        return GoEChargerDataMap::convertTo<uint8_t>(map->find(key));
    }
    return GoEChargerDataMap::invalid<uint8_t>();
}

uint16_t GoEChargerAPI::getUint16(const char* const key) {
    if (map != NULL) {
        return GoEChargerDataMap::convertTo<uint16_t>(map->find(key));
    }
    return GoEChargerDataMap::invalid<uint16_t>();
}

uint32_t GoEChargerAPI::getUint32(const char* const key) {
    if (map != NULL) {
        return GoEChargerDataMap::convertTo<uint32_t>(map->find(key));
    }
    return GoEChargerDataMap::invalid<uint32_t>();
}

#ifdef WIN32
#pragma warning( disable : 4506 )   // warning C4506: No definition for inline function "std::string GoEChargerDataMap::convertTo<std::string>(const json_value &)"
#endif
std::string GoEChargerAPI::getString(const char* const key) {
    if (map != NULL) {
        return GoEChargerDataMap::convertTo<std::string>(map->find(key));
    }
    return GoEChargerDataMap::invalid<std::string>();
}
