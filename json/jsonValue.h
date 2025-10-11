#pragma once
#include <string>

class JsonArray;
class JsonObject;

struct JsonValue {
    enum class Type {
        null,
        boolean,
        string,
        number,
        object,
        array
    };

    JsonValue(const JsonValue& other);
    JsonValue(const JsonValue&& other) noexcept;

    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;

    JsonValue(const std::string& value);
    JsonValue(const JsonObject& value);
    JsonValue(const JsonArray& value);
    JsonValue(std::monostate value);
    JsonValue(double value);
    JsonValue(bool value);

    JsonValue();

    bool operator==(const JsonValue& other) const;

    std::variant<std::monostate, bool, std::string, double, std::unique_ptr<JsonObject>, std::unique_ptr<JsonArray>> value = std::monostate();
    Type type = Type::null;

    [[nodiscard]] std::string valueToString() const;
    [[nodiscard]] std::string typeToString() const;

    operator std::string() const;
    operator JsonObject() const;
    operator JsonArray() const;
    operator double() const;
    operator bool() const;
};