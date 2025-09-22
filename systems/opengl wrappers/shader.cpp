#include "shader.h"

#include <iostream>
#include <sstream>

#include "glad/gl.h"

Shader::Shader(const std::string& vertex, const std::string& fragment) {
    // get the strings from the shader files
    auto const vertexShader = getShaderString(vertex);
    auto const fragmentShader = getShaderString(fragment);
    // compile the shaders
    this->programId_ = createShader(vertexShader, fragmentShader);

    bind();
}

int Shader::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(programId_, name.c_str());
}

int Shader::getProgram() const {
    return programId_;
}

void Shader::bind() const {
    glUseProgram(programId_);
}

Shader::~Shader() {
    glDeleteProgram(programId_);
}

std::string Shader::getShaderString(const std::string& filePath) {
    std::fstream fileStream(filePath);

    std::string line;
    std::stringstream ss;

    while (getline(fileStream, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

unsigned int Shader::compileShader(const unsigned int type, const std::string& source) {
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char message[lenght];
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "Failed to compile shader! \n" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    const unsigned int program = glCreateProgram();

    const unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexShader);
    const unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    // we can delete the shaders because they are already attached to our program
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}
