#include "jsonToken.h"

std::string JsonToken::typeToString() const {
    switch (type) {
        case Type::string:
            return "string";
        case Type::start_object:
            return "start_object";
        case Type::end_object:
            return "end_object";
        case Type::boolean:
            return "boolean";
        case Type::number:
            return "number";
        case Type::start_array:
            return "start_array";
        case Type::end_array:
            return "end_array";
        case Type::null:
            return "null";
        case Type::eof:
            return "eof";
        default:
            return "";
    }
}
