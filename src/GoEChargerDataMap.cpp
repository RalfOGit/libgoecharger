#define _CRT_SECURE_NO_WARNINGS
#include <GoEChargerDataMap.hpp>


GoEChargerDataMap::GoEChargerDataMap(const json_value& _json_) : json(_json_) {}


GoEChargerDataMap::~GoEChargerDataMap(void) {}


const json_value& GoEChargerDataMap::getJson(void) {
    return json;
}


size_t GoEChargerDataMap::size(void) {
    if (json.type == json_object) {
        return json.u.object.length;
    }
    return 0;
}


const json_object_entry& GoEChargerDataMap::at(const size_t index) {
    if (json.type == json_object && index >= 0 && index < json.u.object.length) {
        return json.u.object.values[index];
    }
    return *json_value_none.u.object.values;    // will cause null pointer exception
}


const json_value& GoEChargerDataMap::find(const char *const key) {
    if (json.type == json_object) {
        for (unsigned int i = 0; i < json.u.object.length; ++i) {
            const json_object_entry& entry = json.u.object.values[i];
            if (strcmp(entry.name, key) == 0) {
                return *entry.value;
            }
        }
    }
    return json_value_none;
}


std::string GoEChargerDataMap::toString(void) const {
    return convertTo<std::string>(json);
}


std::string GoEChargerDataMap::toString(const json_object_entry& entry) {
    if (entry.value != NULL) {
        std::string result;
        result.append(entry.name, entry.name_length);
        result.append(": ");
        result.append(convertTo<std::string>(*entry.value));
        result.append("\n");
        return result;
    }
    return "error";
}


template<class T> T GoEChargerDataMap::convertTo(const json_value& json) {
    switch (json.type) {
    case json_string: {
        unsigned long long value = invalid<T>();
        sscanf(json.u.string.ptr, "%llu", &value);
        return (T)value; }
    case json_integer:
        return (T)json.u.integer;
    case json_double:
        return (T)json.u.dbl;
    case json_boolean:
        return (T)json.u.boolean;
    }
    return invalid<T>();
}


template<> double GoEChargerDataMap::convertTo<double>(const json_value& json) {
    switch (json.type) {
    case json_string: {
        double value = invalid<double>();
        sscanf(json.u.string.ptr, "%lf", &value);
        return value; }
    case json_integer:
        return (double)json.u.integer;
    case json_double:
        return (double)json.u.dbl;
    case json_boolean:
        return (double)json.u.boolean;
    }
    return invalid<double>();
}


template<> std::string GoEChargerDataMap::convertTo<std::string>(const json_value& value) {
    char buffer[64];
    std::string result;

    switch (value.type) {
    case json_none:     return "none";
    case json_object:
        result.append("{\n");
        for (unsigned int i = 0; i < value.u.object.length; ++i) {
            json_object_entry& entry = value.u.object.values[i];
            result.append(toString(entry));
        }
        result.append("}");
        return result;
    case json_array:
        result.append("[\n");
        for (unsigned int i = 0; i < value.u.array.length; ++i) {
            json_value* element = value.u.array.values[i];
            if (element != NULL) {
                //result.append(toString(*element));
                result.append(convertTo<std::string>(*element));
                result.append("\n");
            }
        }
        result.append("]");
        return result;
    case json_integer:  snprintf(buffer, sizeof(buffer), "%lld", value.u.integer); return buffer;
    case json_double:   snprintf(buffer, sizeof(buffer), "%lf", value.u.dbl); return buffer; // return std::to_string(value.u.dbl);
    case json_string:   return std::string(value.u.string.ptr, value.u.string.length);
    case json_boolean:  strncpy(buffer, (value.u.boolean != 0 ? "true" : "false"), sizeof(buffer)); return buffer;
    case json_null:     return "null";
    }
    return "error";
}


template<class T, size_t size> std::array<T, size> GoEChargerDataMap::convertToArray(const json_value& value) {
    std::array<T, size> arr;
    if (value.type == json_array && size == value.u.array.length) {
        for (unsigned int i = 0; i < value.u.array.length; ++i) {
            json_value* element = value.u.array.values[i];
            if (element != NULL) {
                arr[i] = convertTo<T>(*element);
            }
        }
    }
    else {
        arr.fill(invalid<T>());
    }
    return arr;
}


// explicit template initializations
template std::array<double,    4> GoEChargerDataMap::convertToArray(const json_value& value);
template std::array<uint32_t, 16> GoEChargerDataMap::convertToArray(const json_value& value);

template uint8_t     GoEChargerDataMap::convertTo(const json_value& json);
template uint16_t    GoEChargerDataMap::convertTo(const json_value& json);
template uint32_t    GoEChargerDataMap::convertTo(const json_value& json);
template double      GoEChargerDataMap::convertTo(const json_value& json);
template std::string GoEChargerDataMap::convertTo(const json_value& json);
