#include "jsonParser.h"

#include "jsonLexer.h"
#include "jsonToken.h"
#include "jsonValue.h"

JsonValue JsonParser::parseValue() {
    const JsonToken token = lexer_.nextToken();

    JsonValue value;

    switch (token.type) {
        case JsonToken::Type::start_object:
            value.type = JsonValue::Type::object;
            value.value = parseObject();
            return value;

        case JsonToken::Type::start_array:
            value.type = JsonValue::Type::array;
            value.value = parseArray();
            return value;

        case JsonToken::Type::number:
            value.type = JsonValue::Type::number;
            value.value = stringToDouble(token.value);
            return value;

        case JsonToken::Type::string:
            value.type = JsonValue::Type::string;
            value.value = token.value;
            return value;

        case JsonToken::Type::boolean:
            value.type = JsonValue::Type::boolean;
            value.value = token.value == "true";
            return value;

        case JsonToken::Type::null:
            value.type = JsonValue::Type::null;
            value.value = std::monostate();
            return value;

        default:
            throw ParserError(std::format("Unexpected Token Type: {}", token.typeToString()));
    }
}

JsonObject JsonParser::parseObject() {
    JsonObject result;

    while (true) {
        JsonToken mustBeEndOrKey = lexer_.nextToken();

        if (mustBeEndOrKey.type == JsonToken::Type::end_object) {
            return result;
        }

        const JsonToken& key = mustBeEndOrKey;

        if (key.type != JsonToken::Type::string) {
            throw ParserError(std::format("JsonObject contains invalid token, expected a key, got: {}", key.typeToString()));
        }

        JsonToken seperator = lexer_.nextToken();

        if (seperator.type != JsonToken::Type::colon) {
            throw ParserError(std::format("JsonObject key not followed by token of type sperator, got: {}", seperator.typeToString()));
        }

        JsonValue value = parseValue();

        result.putValueField(key.value, value);

        JsonToken mustBeEndOrComma = lexer_.nextToken();

        if (mustBeEndOrComma.type == JsonToken::Type::end_object) {
            return result;
        }

        if (mustBeEndOrComma.type != JsonToken::Type::comma) {
            throw ParserError(std::format("JsonObject key not followed by token of type comma, got: {}", mustBeEndOrComma.typeToString()));
        }
    }
}

JsonArray JsonParser::parseArray() {
    JsonArray result;

    while (true) {
        JsonValue value = parseValue();

        result.putValue(value);

        JsonToken mustBeCommaOrEndArray = lexer_.nextToken();

        if (mustBeCommaOrEndArray.type == JsonToken::Type::end_array) {
            return result;
        }

        if (mustBeCommaOrEndArray.type != JsonToken::Type::comma) {
            throw ParserError("JsonValue in JsonArray not followed by ',' or by end of array");
        }
    }
}


double JsonParser::stringToDouble(const std::string& string) {
    double result = 0.0;
    int sign = 1;
    int i = 0;

    if (string[i] == '-') {
        sign = -1;
        i++;
    }

    while (string[i] >= '0' && string[i] <= '9') {
        result = result * 10.00 + (string[i] - '0');
        i++;
    }

    if (string[i] == '.') {
        i++;
        double decimalPlace = 0.1;
        while (string[i] >= '0' && string[i] <= '9') {
            result = result + (string[i] - '0') * decimalPlace;
            decimalPlace /= 10;
            i++;
        }
    }

    if (string[i] == 'e' || string[i] == 'E') {
        int exponent = 0;
        int exponentSign = 1;
        i++;

        if (string[i] == '-') {
            exponentSign = -1;
            i++;
        }

        while (string[i] >= '0' && string[i] <= '9') {
            exponent = exponent * 10 + (string[i] - '0');
            i++;
        }

        result *= std::pow(10, exponent * exponentSign);
    }

    return result * sign;
}
