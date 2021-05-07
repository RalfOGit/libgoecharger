#ifndef __GOECHARGERDATAMAP_HPP__
#define __GOECHARGERDATAMAP_HPP__

#include <Json.h>
#include <cstdint>
#include <string>
#include <array>


class GoEChargerDataMap {
protected:
    const json_value& json;

public:

    static constexpr uint32_t INVALID_UINT32 = 0x7fffffff;
    static constexpr uint32_t INVALID_UINT24 = 0x7fffff;
    static constexpr uint16_t INVALID_UINT16 = 0x7fff;
    static constexpr uint8_t  INVALID_UINT8  = 0x7f;
    static constexpr double   INVALID_DOUBLE = 99999999.9999;
    template<class T> static T    invalid(void) { return T(); }
    template<> static uint8_t     invalid(void) { return INVALID_UINT8;  }
    template<> static uint16_t    invalid(void) { return INVALID_UINT16; }
    template<> static uint32_t    invalid(void) { return INVALID_UINT32; }
    template<> static double      invalid(void) { return INVALID_DOUBLE; }
    template<> static std::string invalid(void) { return "INVALID"; }
    template<class T, size_t size> static std::array<T, size> invalidArray(void) { std::array<T, size> arr; arr.fill(invalid<T>()); return arr; }


    GoEChargerDataMap(const json_value& json);
    ~GoEChargerDataMap(void);
    const json_value& getJson(void);

    size_t size(void);
    const json_object_entry& at(const size_t index);
    const json_value& find(const char *const key);

    template<class T> static T    convertTo(const json_value& value);
    template<> static double      convertTo(const json_value& value);
    template<> static std::string convertTo(const json_value& value);
    template<class T, size_t size> static std::array<T, size> convertToArray(const json_value& value);

    std::string toString(void) const;
    static std::string toString(const json_object_entry& entry);
};

#endif
