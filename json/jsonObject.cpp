#include "jsonObject.h"

#include <format>

#include "jsonValue.h"

JsonObject::JsonObject() {
    this->data_ = {};
}

JsonObject::JsonObject(const JsonValue& val) {
    *this = std::get<JsonObject>(val.value);
}

void JsonObject::putNumberField(const std::string& name, double value) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::number, value));
}

void JsonObject::putStringField(const std::string& name, std::string value) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::number, value));
}

void JsonObject::putBoolField(const std::string& name, bool value) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::number, value));
}

void JsonObject::putNullField(const std::string& name) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::number, std::monostate()));
}

void JsonObject::putArrayField(const std::string& name, JsonArray value) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::number, value));
}

void JsonObject::putObjectField(const std::string& name, JsonObject value) {
    data_.insert_or_assign(name, JsonValue(JsonValue::Type::object, value));
}

void JsonObject::putValueField(const std::string& name, const JsonValue& value) {
    data_.insert_or_assign(name, value);
}

double JsonObject::getNumberField(std::string name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::number)
        throw JsonObjectError(std::format("Tried to access json object field '{}' as a number, real type was {}", name, jsonValue.typeToString()));

    return std::get<double>(jsonValue.value);
}

std::string JsonObject::getStringField(std::string name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::string)
        throw JsonObjectError(std::format("Tried to access json object field '{}' as a string, real type was {}", name, jsonValue.typeToString()));

    return std::get<std::string>(jsonValue.value);
}

bool JsonObject::getBoolField(std::string name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::boolean)
        throw JsonObjectError(std::format("Tried to access json object field '{}' as a boolean, real type was {}", name, jsonValue.typeToString()));

    return std::get<bool>(jsonValue.value);
}

bool JsonObject::getIsNullField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::null)
        return false;

    return true;
}

JsonArray JsonObject::getArrayField(std::string name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::array)
        throw JsonObjectError(std::format("Tried to access json object field '{}' as a jsonArray, real type was {}", name, jsonValue.typeToString()));

    return std::get<JsonArray>(jsonValue.value);
}

std::string JsonObject::toString() const {
    std::string result = "{\n";

    for (const auto& [key, value] : data_) {
        result += "\"" + key + "\": " + value.valueToString() + ",\n";
    }

    // get rid of the last , and \n
    result.pop_back();
    result.pop_back();

    // put back the \n and the }
    result += "\n}";

    return result;
}
