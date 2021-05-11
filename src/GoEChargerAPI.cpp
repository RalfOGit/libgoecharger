#include <GoEChargerAPI.hpp>
#include <HttpClient.hpp>

/*
 * Copyright(C) 2021 RalfO. All rights reserved.
 * https://github.com/RalfOGit/libgoecharger
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditionsand the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Warning: The go-eCharger wallbox uses eeprom memory as non-volatile
 *    storage. This kind of memory supports only a very limited number of
 *    write-cycles. Therefore you must NEVER repetitively set properties/
 *    settings.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
    int http_return_code = http_client.sendHttpGetRequest(charger_url, response, content);

    // check if the http return code is 200 OK
    if (http_return_code == 200) {
        // parse json content and allocate new GoEChargerDataMap to access its content
        json_value* json = json_parse(content.c_str(), content.length());
        if (json != NULL) {
            freeMap();
            map = new GoEChargerDataMap(*json);
            //printf("%s\n", map->toString().c_str());
            return true;
        }
    }
    
    return false;
}

uint8_t GoEChargerAPI::getUint8(const char* const key) {
    if (map != NULL) {
        return GoECharger::convertTo<uint8_t>(map->find(key));
    }
    return GoECharger::invalid<uint8_t>();
}

uint16_t GoEChargerAPI::getUint16(const char* const key) {
    if (map != NULL) {
        return GoECharger::convertTo<uint16_t>(map->find(key));
    }
    return GoECharger::invalid<uint16_t>();
}

uint32_t GoEChargerAPI::getUint32(const char* const key) {
    if (map != NULL) {
        return GoECharger::convertTo<uint32_t>(map->find(key));
    }
    return GoECharger::invalid<uint32_t>();
}

#ifdef WIN32
#pragma warning( disable : 4506 )   // warning C4506: No definition for inline function "std::string GoEChargerDataMap::convertTo<std::string>(const json_value &)"
#endif
std::string GoEChargerAPI::getString(const char* const key) {
    if (map != NULL) {
        return GoECharger::convertTo<std::string>(map->find(key));
    }
    return GoECharger::invalid<std::string>();
}

bool GoEChargerAPI::setUint(const char* const key, const uint32_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", value);
    return setString(key, buffer);
}

bool GoEChargerAPI::setString(const char* const key, const std::string& value) {

    // check if the given value is identical to the value returned by the wallbox; if so, there is no need to write to the wallbox.
    if (map != NULL) {
        std::string device_value = GoECharger::convertTo<std::string>(map->find(key));
        if (device_value == value) {
            return true;
        }
    }

    // assemble host_url + path
    std::string url;
    url.reserve(128);
    url.append(host_url);
    url.append("mqtt?payload=");
    url.append(key);
    url.append("=");
    url.append(value);
    //printf("%s\n", url.c_str());

    // send http put request
    HttpClient http;
    std::string response, content;
    int http_return_code = http.sendHttpPutRequest(url, response, content);
    
    // check if the http return code is 200 OK
    if (http_return_code == 200) {
        // parse json content and allocate new GoEChargerDataMap to access its content
        json_value* json = json_parse(content.c_str(), content.length());
        if (json != NULL) {
            freeMap();
            map = new GoEChargerDataMap(*json);
            //printf("%s\n", map->toString().c_str());
            return true;
        }
    }

    return false;
}
