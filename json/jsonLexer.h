#pragma once
#include <fstream>
#include <sstream>
#include <string>

struct JsonToken;

class JsonLexer {
public:
    class LexerError final : public std::exception {
    public:
        explicit LexerError(const std::string& messege) : messege(messege) {};

        const char* what() const noexcept override {
            return messege.c_str();
        }

    private:
        std::string messege;
    };

    explicit JsonLexer(std::ifstream fileStream);

    JsonToken nextToken();

private:
    std::istringstream stream_;

    int line_;
    int character_;

    void throwIfNotExpected(const std::string& expected);
    std::u8string readString();
    std::u8string readNumber();
};
