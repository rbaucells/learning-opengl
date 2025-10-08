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
