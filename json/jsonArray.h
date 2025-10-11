#pragma once
#include <exception>
#include <string>
#include <vector>

struct JsonValue;
class JsonObject;

class JsonArray {
public:
    class JsonArrayError final : public std::exception {
    public:
        explicit JsonArrayError(std::string message) : message_(std::move(message)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    JsonArray();

    void putNumber(double value);
    void putString(const std::string& value);
    void putBool(bool value);
    void putNull();
    void putArray(const JsonArray& value);
    void putObject(const JsonObject& value);

    void putValue(const JsonValue& value);

    void putNumber(int index, double value);
    void putString(int index, const std::string& value);
    void putBool(int index, bool value);
    void putNull(int index);
    void putArray(int index, const JsonArray& value);
    void putObject(int index, const JsonObject& value);

    void putValue(int index, const JsonValue& value);

    [[nodiscard]] double getNumber(int index) const;
    [[nodiscard]] std::string getString(int index) const;
    [[nodiscard]] bool getBool(int index) const;
    [[nodiscard]] bool getNull(int index) const;
    [[nodiscard]] JsonArray getArray(int index) const;
    [[nodiscard]] JsonObject getObject(int index) const;

    [[nodiscard]] JsonValue getValue(int index) const;
    JsonValue operator[](int index);

    [[nodiscard]] std::string toString() const;

    std::vector<JsonValue>::iterator begin();
    std::vector<JsonValue>::iterator end();

    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;

private:
    std::vector<JsonValue> data_ = {};
};
