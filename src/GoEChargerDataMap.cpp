#define _CRT_SECURE_NO_WARNINGS
#include <GoEChargerDataMap.hpp>


/**
 * Constructor.
 * @param json_ a reference to a json tree as received from the wallbox.
 */
GoEChargerDataMap::GoEChargerDataMap(const json_value& json_) : json(json_) {}


/**
 * Destructor.
 */
GoEChargerDataMap::~GoEChargerDataMap(void) {}


/**
 * Get the encapsulated json reference.
 * @return the json reference
 */
const json_value& GoEChargerDataMap::getJson(void) {
    return json;
}


/**
 * Get the number of top-level properties in the json tree.
 * @return the number of top level properties
 */
size_t GoEChargerDataMap::size(void) {
    if (json.type == json_object) {
        return json.u.object.length;
    }
    return 0;
}


/**
 * Get the top-level property key-value pair at the given index position from the json tree.
 * @param index the index position
 * @return the top-level property key-value pair at the given index position
 */
const json_object_entry& GoEChargerDataMap::at(const size_t index) {
    if (json.type == json_object && index >= 0 && index < json.u.object.length) {
        return json.u.object.values[index];
    }
    return *json_value_none.u.object.values;    // will cause null pointer exception
}


/**
 * Get the top-level property value for the given key from the json tree.
 * @param key the key to search for
 * @return the top-level property value for the given key, or json_value_none if there is no such key.
 */
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


/**
 * Get a string representation of the entire encapsulated json tree
 * @return the string representation of the json tree
 */
std::string GoEChargerDataMap::toString(void) const {
    return GoECharger::convertTo<std::string>(json);
}


/**
 * Get a string representation of the given key-value pair.
 * @param entry the json key-value pair entry
 * @return the string representation of the json entry
 */
std::string GoEChargerDataMap::toString(const json_object_entry& entry) {
    if (entry.value != NULL) {
        std::string result;
        result.append(entry.name, entry.name_length);
        result.append(": ");
        result.append(GoECharger::convertTo<std::string>(*entry.value));
        result.append("\n");
        return result;
    }
    return "error";
}


/**
 * Convert the given json value to type T.
 * This template method relies on the type conversions (called C++ operator sugar) provided in json.h.
 * @param json the json value
 * @return the json value cast to type T
 */
template<class T> T GoECharger::convertTo(const json_value& json) {
    switch (json.type) {
    case json_string: {
        unsigned long long value = invalid<T>();
        if (sscanf(json.u.string.ptr, "%llu", &value) == 1) {
            return (T)value;
        }
        return invalid<T>(); }
    case json_integer:
        return (T)json.u.integer;
    case json_double:
        return (T)json.u.dbl;
    case json_boolean:
        return (T)json.u.boolean;
    }
    return invalid<T>();
}


/**
 * Convert the given json value to type double.
 * This is a template specialization for type double.
 * @param json the json value
 * @return the json value cast to type double
 */
template<> double GoECharger::convertTo<double>(const json_value& json) {
    switch (json.type) {
    case json_string: {
        double value = invalid<double>();
        if (sscanf(json.u.string.ptr, "%lf", &value) == 1) {
            return value;
        }
        return invalid<double>(); }
    case json_integer:
        return (double)json.u.integer;
    case json_double:
        return (double)json.u.dbl;
    case json_boolean:
        return (double)json.u.boolean;
    }
    return invalid<double>();
}


/**
 * Convert the given json value to type std::string.
 * This is a template specialization for type std::string.
 * @param value the json value
 * @return the json value cast to type std::string
 */
template<> std::string GoECharger::convertTo<std::string>(const json_value& value) {
    char buffer[64];
    std::string result;

    switch (value.type) {
    case json_none:     return "none";
    case json_object:
        result.append("{\n");
        for (unsigned int i = 0; i < value.u.object.length; ++i) {
            json_object_entry& entry = value.u.object.values[i];
            result.append(GoEChargerDataMap::toString(entry));
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


/**
 * Convert the given json value to type std::array<T, size> - assuming that the given json value is an array.
 * This method calls the previously implemente conversion methods for type T on each array element.
 * @param value the json value referening an array
 * @return the json value cast to type std::array<T, size>
 */
template<class T, size_t size> std::array<T, size> GoECharger::convertToArray(const json_value& value) {
    std::array<T, size> arr;
    if (value.type == json_array && size == value.u.array.length) {
        for (unsigned int i = 0; i < value.u.array.length; ++i) {
            json_value* element = value.u.array.values[i];
            if (element != NULL) {
                arr[i] = convertTo<T>(*element);
            } else {
                arr[i] = invalid<T>();
            }
        }
    }
    else {
        arr.fill(invalid<T>());
    }
    return arr;
}


// explicit template instantiations
template std::array<double,    4> GoECharger::convertToArray(const json_value& value);
template std::array<uint32_t, 16> GoECharger::convertToArray(const json_value& value);

template uint8_t     GoECharger::convertTo(const json_value& json);
template uint16_t    GoECharger::convertTo(const json_value& json);
template uint32_t    GoECharger::convertTo(const json_value& json);
template double      GoECharger::convertTo(const json_value& json);
template std::string GoECharger::convertTo(const json_value& json);
