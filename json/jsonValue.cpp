#include "jsonValue.h"

std::string JsonValue::typeToString() const {
    switch (type) {
        case Type::string:
            return "string";
        case Type::object:
            return "object";
        case Type::boolean:
            return "boolean";
        case Type::number:
            return "number";
        case Type::array:
            return "array";
        case Type::null:
            return "null";
    }

    return "";
}

std::string JsonValue::valueToString() const {
    switch (type) {
        case Type::string:
            return std::get<std::string>(value);
        case Type::object:
            return std::get<JsonObject>(value).toString();
        case Type::boolean: {
            if (std::get<bool>(value))
                return "true";

            return "false";
        }
        case Type::number: {
            return std::to_string(std::get<double>(value));
        }
        case Type::array:
            return std::get<JsonArray>(value).toString();
        case Type::null:
            return "null";
    }

    return "null";
}
