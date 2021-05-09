#include <GoEChargerAPI.hpp>
#include <HttpClient.hpp>


GoEChargerAPI::GoEChargerAPI(const std::string& url) :
    map(NULL),
    host_url(url)
{
    if (host_url.length() > 0 && host_url[host_url.length() - 1] != '/') {
        host_url.append("/");
    }
}

GoEChargerAPI::~GoEChargerAPI(void) {
    freeMap();
}

void GoEChargerAPI::freeMap(void) {
    if (map != NULL) {
        json_value* json = (json_value*)&map->getJson();
        json_value_free(json);
        map = NULL;
    }
}


bool GoEChargerAPI::refreshMap(void) {

    // assemble host_url + /status path
    std::string charger_url(host_url);
    charger_url.append("status");

    // send http status request
    HttpClient http_client;
    std::string response;
    std::string content;
    http_client.sendHttpGetRequest(charger_url, response, content);

    // parse json content and allocate new GoEChargerDataMap to access its content
    json_value* json = json_parse(content.c_str(), content.length());
    if (json != NULL) {
        freeMap();
        map = new GoEChargerDataMap(*json);
        //printf("%s\n", map->toString().c_str());
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

bool GoEChargerAPI::setUint8(const char* const key, const uint8_t value) {
    return setUint32(key, value);
}

bool GoEChargerAPI::setUint16(const char* const key, const uint16_t value) {
    return setUint32(key, value);
}

bool GoEChargerAPI::setUint32(const char* const key, const uint32_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", value);
    return setString(key, buffer);
}

bool GoEChargerAPI::setString(const char* const key, const std::string& value) {

    // assemble host_url + path
    std::string url;
    url.reserve(128);
    url.append(host_url);
    url.append("mqtt?payload=");
    url.append(key);
    url.append("=");
    url.append(value);

    // send http put request
    HttpClient http;
    std::string response, content;
    int n = http.sendHttpPutRequest(url, response, content);

    // parse json content and allocate new GoEChargerDataMap to access its content
    json_value* json = json_parse(content.c_str(), content.length());
    if (json != NULL) {
        freeMap();
        map = new GoEChargerDataMap(*json);
        //printf("%s\n", map->toString().c_str());
        return true;
    }
    return false;
}
