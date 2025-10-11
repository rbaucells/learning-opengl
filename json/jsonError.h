#pragma once
#include <exception>
#include <string>

class JsonError final : public std::exception {
public:
    explicit JsonError(std::string message) : message_(std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};
