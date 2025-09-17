#pragma once
#include <fstream>
#include <string>

class Shader {
public:
    Shader(const std::string& vertex, const std::string& fragment);

    [[nodiscard]] int getUniformLocation(const std::string& name) const;
    [[nodiscard]] int getProgram() const;

    template<typename... ARGS>
    void setUniformValue(void (*setUniformFunc)(ARGS...), ARGS&&... args) const {
        bind();
        setUniformFunc(std::forward<ARGS>(args)...);
    }

    void bind() const;

    ~Shader();

private:
    int programId_;

    // shading
    static std::string getShaderString(const std::string& filePath);

    static unsigned int compileShader(unsigned int type, const std::string& source);

    static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};
