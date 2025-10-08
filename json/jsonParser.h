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
        explicit ParserError(std::string message) : messege(std::move(message)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return messege.c_str();
        }

    private:
        std::string messege;
    };

    explicit JsonParser(JsonLexer& lexer) : lexer_(lexer) {}

    JsonValue parseValue();

private:
    JsonLexer& lexer_;

    JsonObject parseObject();
    JsonArray parseArray();

    static double stringToDouble(const std::string& string);
};
