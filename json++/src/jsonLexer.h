#pragma once
#include <fstream>
#include <string>
#include <utility>

struct JsonToken;

class JsonLexer {
public:
    class LexerError final : public std::exception {
    public:
        explicit LexerError(std::string  messege) : messege(std::move(messege)) {};

        [[nodiscard]] const char* what() const noexcept override {
            return messege.c_str();
        }

    private:
        std::string messege;
    };

    explicit JsonLexer(std::istream& ss);

    JsonToken nextToken();

private:
    std::istream& stream_;

    int line_ = 1;
    int character_ = 0;

    void throwIfNotExpected(const std::string& expected);
    std::string readString();
    std::string readNumber();

    uint16_t readNext4HexBytesAsInt16();
};
