#include "jsonArray.h"

#include <format>

#include "jsonValue.h"

double JsonArray::getNumber(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::number)
        throw JsonArrayError(std::format("Tried to access json object field at index: {} as a number, real type was {}", index, jsonValue.typeToString()));

    return std::get<double>(jsonValue.value);
}

bool JsonArray::getBool(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::boolean)
        throw JsonArrayError(std::format("Tried to access json object field at index: {} as a bool, real type was {}", index, jsonValue.typeToString()));

    return std::get<bool>(jsonValue.value);
}

bool JsonArray::getNull(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::null)
        return false;

    return true;
}

std::string JsonArray::getString(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::string)
        throw JsonArrayError(std::format("Tried to access json object field at index: {} as a string, real type was {}", index, jsonValue.typeToString()));

    return std::get<std::string>(jsonValue.value);
}

JsonObject JsonArray::getObject(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::object)
        throw JsonArrayError(std::format("Tried to access json object field at index: {} as a jsonObject, real type was {}", index, jsonValue.typeToString()));

    return std::get<JsonObject>(jsonValue.value);
}

JsonArray JsonArray::getArray(const int index) const {
    auto jsonValue = data_.at(index);

    if (jsonValue.type != JsonValue::Type::array)
        throw JsonArrayError(std::format("Tried to access json object field at index: {} as a jsonArray, real type was {}", index, jsonValue.typeToString()));

    return std::get<JsonArray>(jsonValue.value);
}

void JsonArray::putNumber(const double value) {
    data_.emplace_back(JsonValue::Type::number, value);
}

void JsonArray::putString(const std::string& value) {
    data_.emplace_back(JsonValue::Type::string, value);
}

void JsonArray::putBool(const bool value) {
    data_.emplace_back(JsonValue::Type::boolean, value);
}

void JsonArray::putNull() {
    data_.emplace_back(JsonValue::Type::null, std::monostate());
}

void JsonArray::putArray(const JsonArray& value) {
    data_.emplace_back(JsonValue::Type::array, value);
}

void JsonArray::putObject(const JsonObject& value) {
    data_.emplace_back(JsonValue::Type::object, value);
}

void JsonArray::putValue(const JsonValue& value) {
    data_.push_back(value);
}

void JsonArray::putNumber(const int index, const double value) {
    data_.emplace(data_.begin() + index, JsonValue::Type::number, value);
}

void JsonArray::putString(const int index, const std::string& value) {
    data_.emplace(data_.begin() + index, JsonValue::Type::string, value);
}

void JsonArray::putBool(const int index, const bool value) {
    data_.emplace(data_.begin() + index, JsonValue::Type::boolean, value);
}

void JsonArray::putNull(const int index) {
    data_.emplace(data_.begin() + index, JsonValue::Type::null, std::monostate());
}

void JsonArray::putArray(const int index, const JsonArray& value) {
    data_.emplace(data_.begin() + index, JsonValue::Type::array, value);
}

void JsonArray::putObject(const int index, const JsonObject& value) {
    data_.emplace(data_.begin() + index, JsonValue::Type::object, value);
}

void JsonArray::putValue(int index, const JsonValue& value) {
    data_.emplace(data_.begin() + index, value);
}


std::string JsonArray::toString() const {
    std::string result = "[\n";

    for (const auto & i : data_) {
        result += i.valueToString() + ",\n";
    }

    // get rid of the last , and \n
    result.pop_back();
    result.pop_back();

    // put back the \n and the ]
    result += "\n]";

    return result;
}
