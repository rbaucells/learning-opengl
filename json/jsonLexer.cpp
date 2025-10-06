#include "jsonLexer.h"
#include <sstream>
#include "jsonToken.h"
#include <format>

constexpr int UTF8_MIN_NUMBER_DECIMAL_VALUE = 48;
constexpr int UTF8_MAX_NUMBER_DECIMAL_VALUE = 57;

JsonToken JsonLexer::nextToken() {
    JsonToken jsonToken;

    while (stream_.good()) {
        const char8_t c = stream_.get();

        if (c == 0 || stream_.eof()) {
            throw LexerError("Could not find Token");
        }

        if (c == '\n') {
            line_++;
            character_ = 0;
            continue;
        }

        if (c == ' ' || c == '\t' || c == '\r') {
            character_++;
            continue;
        }

        switch (c) {
            // objects and arrays
            case '{':
                jsonToken.type = JsonToken::Type::start_object;
                jsonToken.value = u8"{";
                return jsonToken;
            case '}':
                jsonToken.type = JsonToken::Type::end_object;
                jsonToken.value = u8"}";
                return jsonToken;
            case '[':
                jsonToken.type = JsonToken::Type::start_array;
                jsonToken.value = u8"[";
                return jsonToken;
            case ']':
                jsonToken.type = JsonToken::Type::end_array;
                jsonToken.value = u8"]";
                return jsonToken;

            // special words (true, false, null)
            case 't':
                throwIfNotExpected("rue");
                jsonToken.type = JsonToken::Type::boolean;
                jsonToken.value = u8"true";
                return jsonToken;
            case 'f':
                throwIfNotExpected("alse");
                jsonToken.type = JsonToken::Type::boolean;
                jsonToken.value = u8"false";
                return jsonToken;
            case 'n':
                throwIfNotExpected("ull");
                jsonToken.type = JsonToken::Type::null;
                jsonToken.value = u8"null";
                return jsonToken;

            // number stuff
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                jsonToken.type = JsonToken::Type::number;
                jsonToken.value = c + readNumber();
                return jsonToken;

            // string stuff
            case '"':
                jsonToken.type = JsonToken::Type::string;
                jsonToken.value = readString();
                break;
        }
    }
}

void JsonLexer::throwIfNotExpected(const std::string& expected) {
    for (const char expectedChar : expected) {
        if (const char curChar = stream_.get(); curChar != expectedChar)
            throw LexerError(std::format("Got unexpected char at (line: %d, char: %d) Expected: '%c', Got '%c'", line_, character_, curChar, expectedChar));

        character_++;
    }
}

std::u8string JsonLexer::readNumber() {
    std::u8string result;

    while (stream_.good()) {
        switch (const char c = stream_.get()) {
            case '+':
            case '-':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case 'e':
            case 'E':
            case '.':
                result += c;
                break;
            case '"':
                return result;
                break;
            default:
                throw LexerError(std::format("Invalid Character: '%c', where number is supposed to be (line: %d, char: %d)", c, line_, character_));
                break;
        }
    }

    return result;
}

std::u8string JsonLexer::readString() {
    std::u8string result;

    while (stream_.good()) {
        switch (const char c = stream_.get()) {
            case '"':
                return result;
                break;

            // someones trying to escape something
            case '\\':
                switch (char nextChar = stream_.get()) {
                    case '\"':
                        result += '\"';
                        break;
                    case '\\':
                        result += '\\';
                        break;
                    case 'b':
                        result += '\b';
                        break;
                    case 'f':
                        result += '\f';
                        break;
                    case 'n':
                        result += '\n';
                        break;
                    case 'r':
                        result += '\r';
                        break;
                    case 't':
                        result += '\t';
                        break;
                    case 'u': {
                        break;
                    }
                    default:
                        throw LexerError("Unknown escaped character");
                }
                break;

            default:
                int count;

                if ((c & 0x80) == 0x00) { // its a standard 1 byte ASCII
                    count = 1;
                }
                else if ((c & 0xE0) == 0xC0) { // its a 2 byte character
                    count = 2;
                }
                else if ((c & 0xF0) == 0xE0) { // its a 3 byte character
                    count = 3;
                }
                else if ((c & 0xF0) == 0xE0) { // its a 3 byte character
                    count = 4;
                }
                else {
                    throw LexerError("Invalid utf-8 leading byte");
                }

                result += c;

                for (int i = 1; i < count; ++i) {
                    if (!stream_.good()) {
                        throw LexerError("Stream closed while reading multi-byte character");
                    }

                    const char8_t continuationByte = stream_.get();

                    if ((continuationByte & 0xC0) != 0x80) {
                        throw LexerError("Invalid UTF-8 continuation byte detected");
                    }

                    result += continuationByte;
                }

                break;
        }
    }

    throw LexerError("Stream closed while reading string");
}