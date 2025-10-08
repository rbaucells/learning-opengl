#include "jsonLexer.h"
#include <format>
#include "jsonToken.h"

constexpr int UTF8_MIN_NUMBER_DECIMAL_VALUE = 48;
constexpr int UTF8_MAX_NUMBER_DECIMAL_VALUE = 57;

JsonLexer::JsonLexer(std::istream& ss) : stream_(ss) {
    if (!stream_.good()) {
        throw LexerError("Input stream is not open or readable.");
    }
}

JsonToken JsonLexer::nextToken() {
    JsonToken jsonToken;

    do {
        const int characterInt = stream_.get();

        if (characterInt == -1 || stream_.eof()) {
            jsonToken.type = JsonToken::Type::eof;
            jsonToken.value = "";

            return jsonToken;
        }

        char c = static_cast<char>(characterInt);

        if (c == ' ' ||  c == '\n' || c == '\r' || c == '\t') {
            if (c == '\n' || c == '\r') {
                line_++;
                character_ = 0;
            }
            else {
                character_++;
            }
            continue;
        }

        character_++;

        switch (c) {
            // objects and arrays
            case '{':
                jsonToken.type = JsonToken::Type::start_object;
                jsonToken.value = "{";
                return jsonToken;
            case '}':
                jsonToken.type = JsonToken::Type::end_object;
                jsonToken.value = "}";
                return jsonToken;
            case '[':
                jsonToken.type = JsonToken::Type::start_array;
                jsonToken.value = "[";
                return jsonToken;
            case ']':
                jsonToken.type = JsonToken::Type::end_array;
                jsonToken.value = "]";
                return jsonToken;

            // special words (true, false, null)
            case 't':
                throwIfNotExpected("rue");
                jsonToken.type = JsonToken::Type::boolean;
                jsonToken.value = "true";
                return jsonToken;
            case 'f':
                throwIfNotExpected("alse");
                jsonToken.type = JsonToken::Type::boolean;
                jsonToken.value = "false";
                return jsonToken;
            case 'n':
                throwIfNotExpected("ull");
                jsonToken.type = JsonToken::Type::null;
                jsonToken.value = "null";
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
                return jsonToken;

            // comma and colon
            case ',':
                jsonToken.type = JsonToken::Type::comma;
                jsonToken.value = ',';
                return jsonToken;
            case ':':
                jsonToken.type = JsonToken::Type::colon;
                jsonToken.value = ':';
                return jsonToken;

            // default
            default:
                throw LexerError(std::format("Unexpected character at (line %d, char %d)", line_, character_));
        }
    }
    while (stream_.good());

    jsonToken.type = JsonToken::Type::eof;
    jsonToken.value = "";

    return jsonToken;
}

void JsonLexer::throwIfNotExpected(const std::string& expected) {
    for (const char expectedChar : expected) {
        const int curCharInt = stream_.get();

        if (curCharInt == -1)
            throw LexerError("Got eof char while expecting string");

        character_++;

        if (const char curChar = static_cast<char>(curCharInt); curChar != expectedChar)
            throw LexerError(std::format("Got unexpected char at (line: {}, char: {}) Expected: '{}', Got '{}'", line_, character_, static_cast<char>(curChar), static_cast<char>(expectedChar)));
    }
}

std::string JsonLexer::readString() {
    std::string result;

    while (stream_.good()) {
        const int characterInt = stream_.get();

        // eof
        if (characterInt == -1)
            return result;

        character_++;
        switch (const char c = static_cast<char>(characterInt)) {
            case '"':
                return result;

            // someone's trying to escape something
            case '\\': {
                const int nextCharacterInt = stream_.get();

                // eof
                if (nextCharacterInt == -1)
                    return result += c;

                character_++;
                switch (static_cast<char>(nextCharacterInt)) {
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
                        uint32_t codePoint = readNext4HexBytesAsInt16();

                        if (codePoint >= 0xD800 && codePoint <= 0xDBFF) {
                            // we need to 'consume' the other \u cuz yes
                            if (stream_.get() != '\\' || stream_.get() != 'u') {
                                throw LexerError("High surrogate not followed by a valid \\u escape sequence");
                            }
                            character_ += 2;

                            if (const uint32_t lowSurrogate = readNext4HexBytesAsInt16(); lowSurrogate >= 0xDC00 && lowSurrogate <= 0xDFFF) {
                                // apply the formula: U = 0x10000 + (H - 0xD800) * 0x400 + (L - 0xDC00)
                                uint32_t highBits = (codePoint - 0xD800) * 0x400; // 0x400 = 1024
                                uint32_t lowBits = (lowSurrogate - 0xDC00);
                                codePoint = 0x10000 + highBits + lowBits;
                            }
                            else {
                                throw LexerError("High surrogate not followed by valid low surrogate");
                            }
                        }
                        else if (codePoint >= 0xDC00 && codePoint <= 0xDFFF) {
                            throw LexerError("Lone low surrogate found in \\u escape");
                        }
                        // else the codePoint is not the surrogate stuff

                        if (codePoint <= 0x7F) { // 1-byte
                            result += static_cast<char>(codePoint);
                        }
                        else if (codePoint <= 0x7FF) { // 2-bytes
                            result += static_cast<char>(0xC0 | (codePoint >> 6));
                            result += static_cast<char>(0x80 | (codePoint & 0x3F));
                        }
                        else if (codePoint <= 0xFFFF) { // 3-bytes
                            result += static_cast<char>(0xE0 | (codePoint >> 12));
                            result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
                            result += static_cast<char>(0x80 | (codePoint & 0x3F));
                        }
                        else if (codePoint <= 0x10FFFF) { // 4-bytes
                            result += static_cast<char>(0xF0 | (codePoint >> 18));
                            result += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
                            result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
                            result += static_cast<char>(0x80 | (codePoint & 0x3F));
                        }
                        else {
                            throw LexerError("Unicode code point is outside the valid range (U+0000 to U+10FFFF)");
                        }

                        break;
                    }
                    default:
                        throw LexerError("Unknown escaped character");
                }
                break;
            }

            default:
                int count;

                if ((c & 0x80) == 0x00) { // it's a standard 1 byte ASCII
                    count = 1;
                }
                else if ((c & 0xE0) == 0xC0) { // it's a 2 byte character
                    count = 2;
                }
                else if ((c & 0xF0) == 0xE0) { // it's a 3 byte character
                    count = 3;
                }
                else if ((c & 0xF8) == 0xF0) { // it's a 4 byte character
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

                    const int continuationByteInt = stream_.get();

                    if (continuationByteInt == -1) {
                        throw LexerError("Reached end of file while expecting continuation byte");
                    }

                    const char continuationByte = static_cast<char>(continuationByteInt);

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

std::string JsonLexer::readNumber() {
    std::string result;

    while (stream_.good()) {
        const int characterInt = stream_.get();

        if (characterInt == -1)
            return result;

        switch (const char c = static_cast<char>(characterInt)) {
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
                character_++;
                break;
            case ',':
            case ' ':
                stream_.unget();
                return result;
            case '\n':
                line_++;
                character_ = 0;
                return result;
            default:
                throw LexerError(std::format("Invalid Character: '%c', where number is supposed to be (line: %d, char: %d)", c, line_, character_));
        }
    }

    return result;
}

uint16_t JsonLexer::readNext4HexBytesAsInt16() {
    uint16_t result = 0;

    for (int i = 0; i < 4; ++i) {
        const int charAsInt = stream_.get();

        if (charAsInt == -1) {
            throw LexerError("Reached eof while reading next 4 bytes as int16");
        }

        const char c = static_cast<char>(charAsInt);

        character_++;

        if (!stream_.good()) {
            throw LexerError("Stream closed while reading next 4 hex bytes as int");
        }

        result <<= 4; // shift left by 4 bits because each digit in hex is 4 bits

        // if it's a digit as a char
        if (c >= '0' && c <= '9') {
            result |= (c - '0');
        }
        else if (c >= 'a' && c <= 'f') { // if it's a lowercase character
            result |= (c - 'a' + 10);
        }
        else if (c >= 'A' && c <= 'F') { // if it's an uppercase character
            result |= (c - 'A' + 10);
        }
        else {
            throw LexerError("Unknown hex character while reading hex bytes");
        }
    }

    return result;
}
