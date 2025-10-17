#include "jsonArray.h"

#include <format>

#include "jsonError.h"
#include "jsonObject.h"
#include "jsonValue.h"

JsonArray::JsonArray() {}

JsonArray::JsonArray(std::initializer_list<JsonValue> initializers) {
    for (auto value : initializers) {
        putValue(value);
    }
}

void JsonArray::putNumber(const double value) {
    data_.emplace_back(value);
}

void JsonArray::putString(const std::string& value) {
    data_.emplace_back(value);
}

void JsonArray::putBool(const bool value) {
    data_.emplace_back(value);
}

void JsonArray::putNull() {
    data_.emplace_back(std::monostate());
}

void JsonArray::putArray(const JsonArray& value) {
    data_.emplace_back(value);
}

void JsonArray::putObject(const JsonObject& value) {
    data_.emplace_back(value);
}

void JsonArray::putValue(const JsonValue& value) {
    data_.push_back(value);
}

void JsonArray::putNumber(const int index, const double value) {
    data_.emplace(data_.begin() + index, value);
}

void JsonArray::putString(const int index, const std::string& value) {
    data_.emplace(data_.begin() + index, value);
}

void JsonArray::putBool(const int index, const bool value) {
    data_.emplace(data_.begin() + index, value);
}

void JsonArray::putNull(const int index) {
    data_.emplace(data_.begin() + index, std::monostate());
}

void JsonArray::putArray(const int index, const JsonArray& value) {
    data_.emplace(data_.begin() + index, value);
}

void JsonArray::putObject(const int index, const JsonObject& value) {
    data_.emplace(data_.begin() + index, value);
}

void JsonArray::putValue(int index, const JsonValue& value) {
    data_.emplace(data_.begin() + index, value);
}

double JsonArray::getNumber(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::number)
        throw JsonError("Tried to access json array field at index" + std::to_string(index) + " as a number, real type was: " + jsonValue.typeToString());

    return jsonValue;
}

std::string JsonArray::getString(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::string)
        throw JsonError("Tried to access json array field at index" + std::to_string(index) + " as a string, real type was: " + jsonValue.typeToString());

    return jsonValue;
}

bool JsonArray::getBool(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::boolean)
        throw JsonError("Tried to access json array field at index" + std::to_string(index) + " as a boolean, real type was: " + jsonValue.typeToString());

    return jsonValue;
}

bool JsonArray::getNull(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::null)
        return false;

    return true;
}

JsonArray JsonArray::getArray(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::array)
        throw JsonError("Tried to access json array field at index" + std::to_string(index) + " as a JsonArray, real type was: " + jsonValue.typeToString());

    return *std::get<std::unique_ptr<JsonArray>>(jsonValue.value);
}

JsonObject JsonArray::getObject(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::object)
        throw JsonError("Tried to access json array field at index" + std::to_string(index) + " as a JsonObject, real type was: " + jsonValue.typeToString());

    return *std::get<std::unique_ptr<JsonObject>>(jsonValue.value);;
}

JsonValue JsonArray::getValue(const int index) const {
    return data_.at(index);
}

const JsonValue& JsonArray::operator[](const int index) const {
    return data_[index];
}

JsonValue& JsonArray::operator[](const int index) {
    return data_[index];
}

std::string JsonArray::toString() const {
    std::string result = "[\n";

    for (auto it = data_.begin(); it != data_.end(); ++it) {
        const auto& value = *it;

        if (it != data_.end()) {
            result += value.valueToString() + ",\n";
        }
        else {
            result += value.valueToString();
        }
    }

    result += "\n]";

    return result;
}

std::vector<JsonValue>::iterator JsonArray::begin() {
    return data_.begin();
}

std::vector<JsonValue>::iterator JsonArray::end() {
    return data_.end();
}

bool JsonArray::empty() const {
    return data_.empty();
}

size_t JsonArray::size() const {
    return data_.size();
}
