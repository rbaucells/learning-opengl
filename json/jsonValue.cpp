#include "jsonValue.h"
#include "jsonArray.h"
#include "jsonError.h"
#include "jsonObject.h"

JsonValue::JsonValue(const JsonValue& other) {
    this->type = other.type;

    switch (type) {
        case Type::null:
            value = std::monostate();
            break;
        case Type::string:
            value = std::get<std::string>(value);
            break;
        case Type::boolean:
            value = std::get<bool>(value);
            break;
        case Type::number:
            value = std::get<double>(value);
            break;
        case Type::object:
            value = std::make_unique<JsonObject>(*std::get<std::unique_ptr<JsonObject>>(value));
            break;
        case Type::array:
            value = std::make_unique<JsonArray>(*std::get<std::unique_ptr<JsonArray>>(value));
            break;
        default:
            throw JsonError("Tried to copy construct a JsonValue, invalid type found: " + typeToString());
    }
}

JsonValue::JsonValue(const JsonValue&& other) noexcept {
    this->type = other.type;

    switch (type) {
        case Type::null:
            value = std::monostate();
            break;
        case Type::string:
            value = std::move(std::get<std::string>(value));
            break;
        case Type::boolean:
            value = std::get<bool>(value);
            break;
        case Type::number:
            value = std::get<double>(value);
            break;
        case Type::object:
            value = std::move(std::get<std::unique_ptr<JsonObject>>(value));
            break;
        case Type::array:
            value = std::move(std::get<std::unique_ptr<JsonArray>>(value));
            break;
        default:
            throw JsonError("Tried to move construct a JsonValue, invalid type found: " + typeToString());
    }
}

JsonValue& JsonValue::operator=(const JsonValue& other) {
    if (*this != other) {
        this->type = other.type;

        switch (type) {
            case Type::null:
                value = std::monostate();
                break;
            case Type::string:
                value = std::get<std::string>(value);
                break;
            case Type::boolean:
                value = std::get<bool>(value);
                break;
            case Type::number:
                value = std::get<double>(value);
                break;
            case Type::object:
                value = std::make_unique<JsonObject>(*std::get<std::unique_ptr<JsonObject>>(value));
                break;
            case Type::array:
                value = std::make_unique<JsonArray>(*std::get<std::unique_ptr<JsonArray>>(value));
                break;
            default:
                throw JsonError("Tried to copy construct a JsonValue, invalid type found: " + typeToString());
        }
    }

    return *this;
}

JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
    if (*this != other) {
        this->type = other.type;

        switch (type) {
            case Type::null:
                value = std::monostate();
                break;
            case Type::string:
                value = std::move(std::get<std::string>(value));
                break;
            case Type::boolean:
                value = std::get<bool>(value);
                break;
            case Type::number:
                value = std::get<double>(value);
                break;
            case Type::object:
                value = std::move(std::get<std::unique_ptr<JsonObject>>(value));
                break;
            case Type::array:
                value = std::move(std::get<std::unique_ptr<JsonArray>>(value));
                break;
            default:
                throw JsonError("Tried to move construct a JsonValue, invalid type found: " + typeToString());
        }
    }

    return *this;
}

JsonValue::JsonValue(const std::string& value) {
    this->type = Type::string;
    this->value = value;
}

JsonValue::JsonValue(const JsonObject& value) {
    this->type = Type::object;
    this->value = std::make_unique<JsonObject>(value);;
}

JsonValue::JsonValue(const JsonArray& value) {
    this->type = Type::array;
    this->value = std::make_unique<JsonArray>(value);;
}

JsonValue::JsonValue(std::monostate value) {
    this->type = Type::null;
    this->value = value;
}

JsonValue::JsonValue(double value) {
    this->type = Type::number;
    this->value = value;
}

JsonValue::JsonValue(bool value) {
    this->type = Type::boolean;
    this->value = value;
}

JsonValue::JsonValue() {
    this->type = Type::null;
    this->value = std::monostate();
}

bool JsonValue::operator==(const JsonValue& other) const {
    return type == other.type && value == other.value;
}

std::string JsonValue::valueToString() const {
    switch (type) {
        case Type::string:
            return "\"" + std::get<std::string>(value) + "\"";
        case Type::object:
            return std::get<std::unique_ptr<JsonObject>>(value)->toString();
        case Type::boolean: {
            if (std::get<bool>(value))
                return "true";

            return "false";
        }
        case Type::number: {
            return std::to_string(std::get<double>(value));
        }
        case Type::array:
            return std::get<std::unique_ptr<JsonObject>>(value)->toString();
        case Type::null:
            return "null";
        default:
            throw JsonError("Tried to get valueToString, invalid type found" + std::to_string(static_cast<int>(type)));
    }
}

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
        default:
            throw JsonError("Tried to get typeToString(), invalid type found" + std::to_string(static_cast<int>(type)));
    }
}

JsonValue::operator std::string() const {
    if (type != Type::string) {
        throw JsonError("Tried to convert JsonValue to string, type was:" + typeToString());
    }

    return std::get<std::string>(value);
}

JsonValue::operator JsonObject() const {
    if (type != Type::object) {
        throw JsonError("Tried to convert JsonValue to JsonObject, type was:" + typeToString());
    }

    return *std::get<std::unique_ptr<JsonObject>>(value);
}

JsonValue::operator JsonArray() const {
    if (type != Type::array) {
        throw JsonError("Tried to convert JsonValue to JsonArray, type was:" + typeToString());
    }

    return *std::get<std::unique_ptr<JsonArray>>(value);
}

JsonValue::operator double() const {
    if (type != Type::number) {
        throw JsonError("Tried to convert JsonValue to double, type was:" + typeToString());
    }

    return std::get<double>(value);
}

JsonValue::operator bool() const {
    if (type != Type::boolean) {
        throw JsonError("Tried to convert JsonValue to bool, type was:" + typeToString());
    }

    return std::get<bool>(value);
}