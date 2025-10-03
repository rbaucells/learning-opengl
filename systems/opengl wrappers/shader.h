#pragma once
#include <string>

#include "glad/gl.h"

class Shader {
public:
    enum ShaderType {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
        compute = GL_COMPUTE_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        tesselation_evaluation = GL_TESS_EVALUATION_SHADER,
        tesselation_control = GL_TESS_CONTROL_SHADER
    };

    static std::shared_ptr<Shader> create(ShaderType type, const std::string& filePath);

    [[nodiscard]] GLuint getShaderId() const;

    ~Shader();

private:
    static std::string readShaderFile(const std::string& filePath);

    Shader(ShaderType type, const std::string& filePath);

    GLuint shader_;
};
