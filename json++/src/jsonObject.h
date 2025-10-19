#pragma once
#include <string>
#include <unordered_map>
#include "jsonValue.h"

class JsonArray;

class JsonObject {
public:
    void putNumberField(const std::string& name, double value);
    void putStringField(const std::string& name, const std::string& value);
    void putBoolField(const std::string& name, bool value);
    void putNullField(const std::string& name);
    void putArrayField(const std::string& name, const JsonArray& value);
    void putObjectField(const std::string& name, const JsonObject& value);
    void putValueField(const std::string& name, const JsonValue& value);

    [[nodiscard]] double getNumberField(const std::string& name) const;
    [[nodiscard]] std::string getStringField(const std::string& name) const;
    [[nodiscard]] bool getBoolField(const std::string& name) const;
    [[nodiscard]] bool getIsNullField(const std::string& name) const;
    [[nodiscard]] JsonArray getArrayField(const std::string& name) const;
    [[nodiscard]] JsonObject getObjectField(const std::string& name) const;
    [[nodiscard]] JsonValue getValueField(const std::string& name);

    const JsonValue& operator[](const std::string& key) const;
    JsonValue& operator[](const std::string& key);

    [[nodiscard]] std::string toString() const;

private:
    std::unordered_map<std::string, JsonValue> data_ = {};
};
