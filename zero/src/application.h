#pragma once
#include <ostream>
#include "glad/gl.h"
#include "systems/opengl-wrappers/window.h"

class Application {
public:
    class ApplicationError : public std::exception {
    public:
        explicit ApplicationError(std::string message) : message_(std::move(message)) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    Application(const std::string& scenePath);

    void run();

    ~Application();

    std::unique_ptr<Window> window;

private:
    // callbacks
    static void errorCallback(const int error, const char* description);

    static void debugErrorCallback(GLenum source, GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParam);
};
