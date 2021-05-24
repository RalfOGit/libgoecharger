#ifndef __LIBGOECHARGER_GOECHARGERDATAMAP_HPP__
#define __LIBGOECHARGER_GOECHARGERDATAMAP_HPP__

#include <Json.h>
#include <cstdint>
#include <string>
#include <array>

namespace libgoecharger {

    /**
     * Namespace definitions to implement specialized template conversions and invalid value methods in a compatible way
     */
    namespace GoECharger {
        // static definitions of invalid data
        static constexpr uint32_t INVALID_UINT32 = 0x7fffffff;
        static constexpr uint32_t INVALID_UINT24 = 0x7fffff;
        static constexpr uint16_t INVALID_UINT16 = 0x7fff;
        static constexpr uint8_t  INVALID_UINT8 = 0x7f;
        static constexpr double   INVALID_DOUBLE = 99999999.9999;

        // template definitions of invalid data
        template<class T> inline T    invalid(void) { return T(); }
        template<> inline uint8_t     invalid(void) { return INVALID_UINT8; }
        template<> inline uint16_t    invalid(void) { return INVALID_UINT16; }
        template<> inline uint32_t    invalid(void) { return INVALID_UINT32; }
        template<> inline double      invalid(void) { return INVALID_DOUBLE; }
        template<> inline std::string invalid(void) { return "INVALID"; }
        template<class T, size_t size> inline std::array<T, size> invalidArray(void) { std::array<T, size> arr; arr.fill(invalid<T>()); return arr; }

        // template conversion methods
        template<class T> T    convertTo(const json_value& value);
        template<> double      convertTo(const json_value& value);
        template<> std::string convertTo(const json_value& value);
        template<class T, size_t size> std::array<T, size> convertToArray(const json_value& value);
    }

    /**
     * Class implementing a key value map of go-eCharger wallbox status properties/settings.
     * The constructor takes a reference to a json tree as received from the wallbox.
     * The class encapsulate the json tree and provides accessor and conversion methods
     * to simplify usage of the json tree in the context of the wallbox application, where
     * the set of properties is represented as a flat list of json properties.
     */
    class GoEChargerDataMap {
    protected:
        const json_value& json;

    public:
        GoEChargerDataMap(const json_value& json);
        ~GoEChargerDataMap(void);
        const json_value& getJson(void);

        size_t size(void);
        const json_object_entry& at(const size_t index);
        const json_value& find(const char* const key);

        std::string toString(void) const;
        static std::string toString(const json_object_entry& entry);
        static std::string toString(const json_value& value) { return GoECharger::convertTo<std::string>(value); }
    };

}   // namespace goecharger

#endif
