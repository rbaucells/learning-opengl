#include "shaderProgram.h"
#include <fstream>
#include <iostream>

#include "shader.h"
#include <vector>

std::shared_ptr<ShaderProgram> ShaderProgram::create() {
    return std::shared_ptr<ShaderProgram>(new ShaderProgram());
}

std::shared_ptr<ShaderProgram> ShaderProgram::create(std::initializer_list<std::shared_ptr<Shader>> shaders) {
    auto* ptr = new ShaderProgram();

    for (auto& shader : shaders) {
        ptr->attach(shader);
    }

    ptr->linkAndVerify();

    return std::shared_ptr<ShaderProgram>(ptr);
}


void ShaderProgram::attach(const std::shared_ptr<Shader>& shader) {
    glAttachShader(program_, shader->getShaderId());
    shaderObjects_.push_back(shader);
}

void ShaderProgram::detach(const std::shared_ptr<Shader>& shader) {
    glDetachShader(program_, shader->getShaderId());

    for (auto it = shaderObjects_.begin(); it != shaderObjects_.end(); ) {
        if ((*it)->getShaderId() == shader->getShaderId()) {
            shaderObjects_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ShaderProgram::linkAndVerify() const {
    glLinkProgram(program_);

    int linkResult;
    glGetProgramiv(program_, GL_COMPILE_STATUS, &linkResult);

    if (linkResult == GL_FALSE) {
        int length;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetProgramInfoLog(program_, length, &length, message);
        std::cerr << "Failed to link shader program! \n" << message << std::endl;
        glDeleteProgram(program_);
        exit(1);
    }

    glValidateProgram(program_);

    int validationResult;
    glGetProgramiv(program_, GL_COMPILE_STATUS, &validationResult);

    if (validationResult == GL_FALSE) {
        int length;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetProgramInfoLog(program_, length, &length, message);
        std::cerr << "Failed to validate shader program! \n" << message << std::endl;
        glDeleteProgram(program_);
        exit(1);
    }
}

void ShaderProgram::bind() const {
    glUseProgram(program_);
}

int ShaderProgram::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(program_, name.c_str());
}

ShaderProgram::ShaderProgram() {
    this->program_ = glCreateProgram();
}

#pragma region setters
#pragma region ints
void ShaderProgram::setUniformInt(const GLint location, const GLint v0) const {
    glProgramUniform1i(program_, location, v0);
}

void ShaderProgram::setUniformInt(const GLint location, const GLint v0, const GLint v1) const {
    glProgramUniform2i(program_, location, v0, v1);
}

void ShaderProgram::setUniformInt(const GLint location, const GLint v0, const GLint v1, const GLint v2) const {
    glProgramUniform3i(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformInt(const GLint location, const GLint v0, const GLint v1, const GLint v2, const GLint v3) const {
    glProgramUniform4i(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region unsigned ints
void ShaderProgram::setUniformUint(const GLint location, const GLuint v0) const {
    glProgramUniform1ui(program_, location, v0);
}

void ShaderProgram::setUniformUint(const GLint location, const GLuint v0, const GLuint v1) const {
    glProgramUniform2ui(program_, location, v0, v1);
}

void ShaderProgram::setUniformUint(const GLint location, const GLuint v0, const GLuint v1, const GLuint v2) const {
    glProgramUniform3ui(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformUint(const GLint location, const GLuint v0, const GLuint v1, const GLuint v2, const GLuint v3) const {
    glProgramUniform4ui(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region floats
void ShaderProgram::setUniformFloat(const GLint location, const GLfloat v0) const {
    glProgramUniform1f(program_, location, v0);
}

void ShaderProgram::setUniformFloat(const GLint location, const GLfloat v0, const GLfloat v1) const {
    glProgramUniform2f(program_, location, v0, v1);
}

void ShaderProgram::setUniformFloat(const GLint location, const GLfloat v0, const GLfloat v1, const GLfloat v2) const {
    glProgramUniform3f(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformFloat(const GLint location, const GLfloat v0, const GLfloat v1, const GLfloat v2, const GLfloat v3) const {
    glProgramUniform4f(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region doubles
void ShaderProgram::setUniformDouble(const GLint location, const GLdouble v0) const {
    glProgramUniform1d(program_, location, v0);
}

void ShaderProgram::setUniformDouble(const GLint location, const GLdouble v0, const GLdouble v1) const {
    glProgramUniform2d(program_, location, v0, v1);
}

void ShaderProgram::setUniformDouble(const GLint location, const GLdouble v0, const GLdouble v1, const GLdouble v2) const {
    glProgramUniform3d(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformDouble(const GLint location, const GLdouble v0, const GLdouble v1, const GLdouble v2, const GLdouble v3) const {
    glProgramUniform4d(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region int arrays
void ShaderProgram::setUniformIntArray(const GLint location, const GLsizei count, const  GLint* value) const {
    glProgramUniform1iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec2Array(const GLint location, const GLsizei count, const  GLint* value) const {
    glProgramUniform2iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec3Array(const GLint location, const GLsizei count, const  GLint* value) const {
    glProgramUniform3iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec4Array(const GLint location, const GLsizei count, const  GLint* value) const {
    glProgramUniform4iv(program_, location, count, value);
}
#pragma endregion
#pragma region unsigned int arrays
void ShaderProgram::setUniformUintArray(const GLint location, const GLsizei count, const  GLuint* value) const {
    glProgramUniform1uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec2Array(const GLint location, const GLsizei count, const  GLuint* value) const {
    glProgramUniform2uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec3Array(const GLint location, const GLsizei count, const  GLuint* value) const {
    glProgramUniform3uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec4Array(const GLint location, const GLsizei count, const  GLuint* value) const {
    glProgramUniform4uiv(program_, location, count, value);
}
#pragma endregion
#pragma region float arrays
void ShaderProgram::setUniformFloatArray(const GLint location, const GLsizei count, const  GLfloat* value) const {
    glProgramUniform1fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec2Array(const GLint location, const GLsizei count, const  GLfloat* value) const {
    glProgramUniform2fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec3Array(const GLint location, const GLsizei count, const  GLfloat* value) const {
    glProgramUniform3fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec4Array(const GLint location, const GLsizei count, const  GLfloat* value) const {
    glProgramUniform4fv(program_, location, count, value);
}
#pragma endregion
#pragma region double arrays
void ShaderProgram::setUniformDoubleArray(const GLint location, const GLsizei count, const  GLdouble* value) const {
    glProgramUniform1dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec2Array(const GLint location, const GLsizei count, const  GLdouble* value) const {
    glProgramUniform2dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec3Array(const GLint location, const GLsizei count, const  GLdouble* value) const {
    glProgramUniform3dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec4Array(const GLint location, const GLsizei count, const  GLdouble* value) const {
    glProgramUniform4dv(program_, location, count, value);
}
#pragma endregion
#pragma region float square matrices
void ShaderProgram::setUniformFloatMat2(const GLint location, const Matrix<2, 2>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3(const GLint location, const Matrix<3, 3>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4(const GLint location, const Matrix<4, 4>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4fv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region float non-square matrices
void ShaderProgram::setUniformFloatMat2x3(const GLint location, const Matrix<2, 3>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2x3fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3x2(const GLint location, const Matrix<3, 2>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3x2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat2x4(const GLint location, const Matrix<2, 4>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2x4fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4x2(const GLint location, const Matrix<4, 2>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4x2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3x4(const GLint location, const Matrix<3, 4>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3x4fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4x3(const GLint location, const Matrix<4, 3>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4x3fv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region float square matrix arrays
void ShaderProgram::setUniformFloatMat2Array(const GLint location, const GLsizei count, const  GLfloat* value, const GLboolean transpose) const {
    glProgramUniformMatrix2fv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformFloatMat3Array(const GLint location, const GLsizei count, const  GLfloat* value, const GLboolean transpose) const {
    glProgramUniformMatrix3fv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformFloatMat4Array(const GLint location, const GLsizei count, const  GLfloat* value, const GLboolean transpose) const {
    glProgramUniformMatrix4fv(program_, location, count, transpose, value);
}
#pragma endregion
#pragma region double square matrices
void ShaderProgram::setUniformDoubleMat2(const GLint location, const Matrix<2, 2, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3(const GLint location, const Matrix<3, 3, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4(const GLint location, const Matrix<4, 4, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4dv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region double non-square matrices
void ShaderProgram::setUniformDoubleMat2x3(const GLint location, const Matrix<2, 3, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2x3dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3x2(const GLint location, const Matrix<3, 2, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3x2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat2x4(const GLint location, const Matrix<2, 4, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix2x4dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4x2(const GLint location, const Matrix<4, 2, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4x2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3x4(const GLint location, const Matrix<3, 4, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix3x4dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4x3(const GLint location, const Matrix<4, 3, double>& matrix, const GLboolean transpose) const {
    glProgramUniformMatrix4x3dv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region double square matrix arrays
void ShaderProgram::setUniformDoubleMat2Array(const GLint location, const GLsizei count, const  GLdouble* value, const GLboolean transpose) const {
    glProgramUniformMatrix2dv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformDoubleMat3Array(const GLint location, const GLsizei count, const  GLdouble* value, const GLboolean transpose) const {
    glProgramUniformMatrix3dv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformDoubleMat4Array(const GLint location, const GLsizei count, const  GLdouble* value, const GLboolean transpose) const {
    glProgramUniformMatrix4dv(program_, location, count, transpose, value);
}
#pragma endregion
#pragma endregion
#pragma region getters
#pragma region ints
GLint ShaderProgram::getUniformInt(GLint location) const {
    GLint v = 0;
    glGetUniformiv(program_, location, &v);
    return v;
}

void ShaderProgram::getUniformInt(GLint location, GLint* data) const {
    glGetUniformiv(program_, location, data);
}
#pragma endregion
#pragma region unsigned ints
GLuint ShaderProgram::getUniformUint(GLint location) const {
    GLuint v = 0;
    glGetUniformuiv(program_, location, &v);
    return v;
}

void ShaderProgram::getUniformUint(GLint location, GLuint* data) const {
    glGetUniformuiv(program_, location, data);
}
#pragma endregion
#pragma region floats
GLfloat ShaderProgram::getUniformFloat(GLint location) const {
    GLfloat v = 0.0f;
    glGetUniformfv(program_, location, &v);
    return v;
}

void ShaderProgram::getUniformFloat(GLint location, GLfloat* data) const {
    glGetUniformfv(program_, location, data);
}
#pragma endregion
#pragma region doubles
GLdouble ShaderProgram::getUniformDouble(GLint location) const {
    GLdouble v = 0.0;
    glGetUniformdv(program_, location, &v);
    return v;
}

void ShaderProgram::getUniformDouble(GLint location, GLdouble* data) const {
    glGetUniformdv(program_, location, data);
}
#pragma endregion
#pragma region int arrays
void ShaderProgram::getUniformIntArray(GLint location, GLint* data) const {
    glGetUniformiv(program_, location, data);
}

void ShaderProgram::getUniformIntVec2Array(GLint location, GLint* data) const {
    glGetUniformiv(program_, location, data);
}

void ShaderProgram::getUniformIntVec3Array(GLint location, GLint* data) const {
    glGetUniformiv(program_, location, data);
}

void ShaderProgram::getUniformIntVec4Array(GLint location, GLint* data) const {
    glGetUniformiv(program_, location, data);
}
#pragma endregion
#pragma region unsigned int arrays
void ShaderProgram::getUniformUintArray(GLint location, GLuint* data) const {
    glGetUniformuiv(program_, location, data);
}

void ShaderProgram::getUniformUintVec2Array(GLint location, GLuint* data) const {
    glGetUniformuiv(program_, location, data);
}

void ShaderProgram::getUniformUintVec3Array(GLint location, GLuint* data) const {
    glGetUniformuiv(program_, location, data);
}

void ShaderProgram::getUniformUintVec4Array(GLint location, GLuint* data) const {
    glGetUniformuiv(program_, location, data);
}
#pragma endregion
#pragma region float arrays
void ShaderProgram::getUniformFloatArray(GLint location, GLfloat* data) const {
    glGetUniformfv(program_, location, data);
}

void ShaderProgram::getUniformFloatVec2Array(GLint location, GLfloat* data) const {
    glGetUniformfv(program_, location, data);
}

void ShaderProgram::getUniformFloatVec3Array(GLint location, GLfloat* data) const {
    glGetUniformfv(program_, location, data);
}

void ShaderProgram::getUniformFloatVec4Array(GLint location, GLfloat* data) const {
    glGetUniformfv(program_, location, data);
}
#pragma endregion
#pragma region double arrays
void ShaderProgram::getUniformDoubleArray(GLint location, GLdouble* data) const {
    glGetUniformdv(program_, location, data);
}

void ShaderProgram::getUniformDoubleVec2Array(GLint location, GLdouble* data) const {
    glGetUniformdv(program_, location, data);
}

void ShaderProgram::getUniformDoubleVec3Array(GLint location, GLdouble* data) const {
    glGetUniformdv(program_, location, data);
}

void ShaderProgram::getUniformDoubleVec4Array(GLint location, GLdouble* data) const {
    glGetUniformdv(program_, location, data);
}
#pragma endregion
#pragma region float square matrixes
Matrix<2, 2> ShaderProgram::getUniformFloatMat2(GLint location) const {
    Matrix<2,2> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<3, 3> ShaderProgram::getUniformFloatMat3(GLint location) const {
    Matrix<3,3> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<4, 4> ShaderProgram::getUniformFloatMat4(GLint location) const {
    Matrix<4,4> m;
    glGetUniformfv(program_, location, m);
    return m;
}
#pragma endregion
#pragma region float non-square matrixes
Matrix<2, 3> ShaderProgram::getUniformFloatMat2x3(GLint location) const {
    Matrix<2,3> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<3, 2> ShaderProgram::getUniformFloatMat3x2(GLint location) const {
    Matrix<3,2> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<2, 4> ShaderProgram::getUniformFloatMat2x4(GLint location) const {
    Matrix<2,4> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<4, 2> ShaderProgram::getUniformFloatMat4x2(GLint location) const {
    Matrix<4,2> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<3, 4> ShaderProgram::getUniformFloatMat3x4(GLint location) const {
    Matrix<3,4> m;
    glGetUniformfv(program_, location, m);
    return m;
}

Matrix<4, 3> ShaderProgram::getUniformFloatMat4x3(GLint location) const {
    Matrix<4,3> m;
    glGetUniformfv(program_, location, m);
    return m;
}
#pragma endregion
#pragma region float square matrix arrays
void ShaderProgram::getUniformFloatMat2Array(GLint location, GLfloat* value) const {
    glGetUniformfv(program_, location, value);
}

void ShaderProgram::getUniformFloatMat3Array(GLint location, GLfloat* value) const {
    glGetUniformfv(program_, location, value);
}

void ShaderProgram::getUniformFloatMat4Array(GLint location, GLfloat* value) const {
    glGetUniformfv(program_, location, value);
}
#pragma endregion
#pragma region double square matrixes
Matrix<2, 2, double> ShaderProgram::getUniformDoubleMat2(GLint location) const {
    Matrix<2,2,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<3, 3, double> ShaderProgram::getUniformDoubleMat3(GLint location) const {
    Matrix<3,3,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<4, 4, double> ShaderProgram::getUniformDoubleMat4(GLint location) const {
    Matrix<4,4,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}
#pragma endregion
#pragma region double non-square matrixes
Matrix<2, 3, double> ShaderProgram::getUniformDoubleMat2x3(GLint location) const {
    Matrix<2,3,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<3, 2, double> ShaderProgram::getUniformDoubleMat3x2(GLint location) const {
    Matrix<3,2,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<2, 4, double> ShaderProgram::getUniformDoubleMat2x4(GLint location) const {
    Matrix<2,4,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<4, 2, double> ShaderProgram::getUniformDoubleMat4x2(GLint location) const {
    Matrix<4,2,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<3, 4, double> ShaderProgram::getUniformDoubleMat3x4(GLint location) const {
    Matrix<3,4,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}

Matrix<4, 3, double> ShaderProgram::getUniformDoubleMat4x3(GLint location) const {
    Matrix<4,3,double> m;
    glGetUniformdv(program_, location, m);
    return m;
}
#pragma endregion
#pragma region double square matrix arrays
void ShaderProgram::getUniformDoubleMat2Array(GLint location, GLdouble* value) const {
    glGetUniformdv(program_, location, value);
}

void ShaderProgram::getUniformDoubleMat3Array(GLint location, GLdouble* value) const {
    glGetUniformdv(program_, location, value);
}

void ShaderProgram::getUniformDoubleMat4Array(GLint location, GLdouble* value) const {
    glGetUniformdv(program_, location, value);
}
#pragma endregion
#pragma endregion

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}
