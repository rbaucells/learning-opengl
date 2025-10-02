#include "betterShader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/gl.h"

BetterShader::BetterShader(const ShaderType type, const std::string& filePath) {
    const auto fileContents = readShaderFile(filePath);

    shader_ = glCreateShader(type);
    const char* src = fileContents.c_str();

    glShaderSource(shader_, 1, &src, nullptr);
    glCompileShader(shader_);

    int result;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(shader_, length, &length, message);
        std::cerr << "Failed to compile shader! \n" << message << std::endl;
        glDeleteShader(shader_);
        exit(1);
    }
}

GLuint BetterShader::getShaderId() const {
    return shader_;
}

BetterShader::~BetterShader() {
    glDeleteShader(shader_);
}

std::string BetterShader::readShaderFile(const std::string& filePath) {
    std::fstream fileStream(filePath);

    std::string line;
    std::stringstream ss;

    while (getline(fileStream, line)) {
        ss << line << '\n';
    }

    return ss.str();
}
