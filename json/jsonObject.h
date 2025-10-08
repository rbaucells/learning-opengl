#pragma once
#include <string>
#include <unordered_map>
#include <utility>

struct JsonValue;
class JsonArray;

class JsonObject {
public:
    class JsonObjectError final : public std::exception {
    public:
        explicit JsonObjectError(std::string messege) : messege(std::move(messege)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return messege.c_str();
        }

    private:
        std::string messege;
    };

    void putNumberField(const std::string& name, double value);
    void putStringField(const std::string& name, std::string value);
    void putBoolField(const std::string& name, bool value);
    void putNullField(const std::string& name);
    void putArrayField(const std::string& name, JsonArray value);
    void putObjectField(const std::string& name, JsonObject value);

    void putValueField(const std::string& name, const JsonValue& value);

    [[nodiscard]] double getNumberField(std::string name) const;
    [[nodiscard]] std::string getStringField(std::string name) const;
    [[nodiscard]] bool getBoolField(std::string name) const;
    [[nodiscard]] bool getIsNullField(const std::string& name) const;
    [[nodiscard]] JsonArray getArrayField(std::string name) const;

    [[nodiscard]] std::string toString() const;

private:
    std::unordered_map<std::string, JsonValue> data_;
};
