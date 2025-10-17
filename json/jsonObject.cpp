#include "jsonObject.h"
#include <format>

#include "jsonArray.h"
#include "jsonError.h"
#include "jsonValue.h"

void JsonObject::putNumberField(const std::string& name, double value) {
    data_.insert_or_assign(name, value);
}

void JsonObject::putStringField(const std::string& name, const std::string& value) {
    data_.insert_or_assign(name, value);
}

void JsonObject::putBoolField(const std::string& name, bool value) {
    data_.insert_or_assign(name, value);
}

void JsonObject::putNullField(const std::string& name) {
    data_.insert_or_assign(name, std::monostate());
}

void JsonObject::putArrayField(const std::string& name, const JsonArray& value) {
    data_.insert_or_assign(name, value);
}

void JsonObject::putObjectField(const std::string& name, const JsonObject& value) {
    data_.insert_or_assign(name, value);
}

void JsonObject::putValueField(const std::string& name, const JsonValue& value) {
    data_.insert_or_assign(name, value);
}

double JsonObject::getNumberField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type == JsonValue::Type::null) {
        return 0.0;
    }

    if (jsonValue.type != JsonValue::Type::number)
        throw JsonError("Tried to access json object field '" + name + "' as a number, real type was: " + jsonValue.typeToString());

    return std::get<double>(jsonValue.value);
}

std::string JsonObject::getStringField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type == JsonValue::Type::null) {
        return "";
    }

    if (jsonValue.type != JsonValue::Type::string)
        throw JsonError("Tried to access json object field '" + name + "' as a string, real type was: " + jsonValue.typeToString());

    return std::get<std::string>(jsonValue.value);
}

bool JsonObject::getBoolField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type == JsonValue::Type::null) {
        return false;
    }

    if (jsonValue.type != JsonValue::Type::boolean)
        throw JsonError("Tried to access json object field '" + name + "' as a bool, real type was: " + jsonValue.typeToString());

    return std::get<bool>(jsonValue.value);
}

bool JsonObject::getIsNullField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type != JsonValue::Type::null)
        return false;

    return true;
}

JsonArray JsonObject::getArrayField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type == JsonValue::Type::null) {
        return {};
    }

    if (jsonValue.type != JsonValue::Type::array)
        throw JsonError("Tried to access json object field '" + name + "' as a JsonArray, real type was: " + jsonValue.typeToString());

    return *std::get<std::unique_ptr<JsonArray>>(jsonValue.value);
}

JsonObject JsonObject::getObjectField(const std::string& name) const {
    const auto jsonValue = data_.at(name);

    if (jsonValue.type == JsonValue::Type::null) {
        return {};
    }

    if (jsonValue.type != JsonValue::Type::object)
        throw JsonError("Tried to access json object field '" + name + "' as a JsonObject, real type was: " + jsonValue.typeToString());

    return *std::get<std::unique_ptr<JsonObject>>(jsonValue.value);
}

JsonValue JsonObject::getValueField(const std::string& name) {
    return data_.at(name);
}

const JsonValue& JsonObject::operator[](const std::string& key) const {
    return data_.at(key);
}

JsonValue& JsonObject::operator[](const std::string& key) {
    return data_[key];
}

std::string JsonObject::toString() const {
    std::string result = "{\n";

    for (auto it = data_.begin(); it != data_.end(); ++it) {
        const auto& [key, value] = *it;

        if (it != data_.end()) {
            result += "\"" + key + "\": " + value.valueToString() + ",\n";
        }
        else {
            result += "\"" + key + "\": " + value.valueToString();
        }
    }

    result += "\n}";

    return result;
}
