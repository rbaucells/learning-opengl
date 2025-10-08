#pragma once
#include <exception>
#include <string>
#include <utility>

struct JsonValue;
class JsonArray;
class JsonObject;
class JsonLexer;
struct JsonToken;

class JsonParser {
public:
    class ParserError final : public std::exception {
    public:
        explicit ParserError(std::string message) : message_(std::move(message)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    explicit JsonParser(JsonLexer& lexer) : lexer_(lexer) {}

    JsonValue parseValue();

private:
    JsonLexer& lexer_;

    JsonObject parseObject();
    JsonArray parseArray();

    static double stringToDouble(const std::string& string);
};
