#include "shaderProgram.h"
#include <fstream>
#include <iostream>

#include "betterShader.h"

ShaderProgram::ShaderProgram() {
    this->program_ = glCreateProgram();
}

void ShaderProgram::attach(const BetterShader& shader) {
    glAttachShader(program_, shader.getShaderId());
}

void ShaderProgram::linkAndVerify() {
    glLinkProgram(program_);
    glValidateProgram(program_);

    int result;
    glGetProgramiv(program_, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetProgramInfoLog(program_, length, &length, message);
        std::cerr << "Failed to compile shader! \n" << message << std::endl;
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

#pragma region setters
#pragma region ints
void ShaderProgram::setUniformInt(GLint location, GLint v0) {
    glProgramUniform1i(program_, location, v0);
}

void ShaderProgram::setUniformInt(GLint location, GLint v0, GLint v1) {
    glProgramUniform2i(program_, location, v0, v1);
}

void ShaderProgram::setUniformInt(GLint location, GLint v0, GLint v1, GLint v2) {
    glProgramUniform3i(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformInt(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    glProgramUniform4i(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region unsigned ints
void ShaderProgram::setUniformUint(GLint location, GLuint v0) {
    glProgramUniform1ui(program_, location, v0);
}

void ShaderProgram::setUniformUint(GLint location, GLuint v0, GLuint v1) {
    glProgramUniform2ui(program_, location, v0, v1);
}

void ShaderProgram::setUniformUint(GLint location, GLuint v0, GLuint v1, GLuint v2) {
    glProgramUniform3ui(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformUint(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    glProgramUniform4ui(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region floats
void ShaderProgram::setUniformFloat(GLint location, GLfloat v0) {
    glProgramUniform1f(program_, location, v0);
}

void ShaderProgram::setUniformFloat(GLint location, GLfloat v0, GLfloat v1) {
    glProgramUniform2f(program_, location, v0, v1);
}

void ShaderProgram::setUniformFloat(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    glProgramUniform3f(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformFloat(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glProgramUniform4f(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region doubles
void ShaderProgram::setUniformDouble(GLint location, GLdouble v0) {
    glProgramUniform1d(program_, location, v0);
}

void ShaderProgram::setUniformDouble(GLint location, GLdouble v0, GLdouble v1) {
    glProgramUniform2d(program_, location, v0, v1);
}

void ShaderProgram::setUniformDouble(GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
    glProgramUniform3d(program_, location, v0, v1, v2);
}

void ShaderProgram::setUniformDouble(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
    glProgramUniform4d(program_, location, v0, v1, v2, v3);
}
#pragma endregion
#pragma region int arrays
void ShaderProgram::setUniformIntArray(GLint location, GLsizei count, const GLint* value) {
    glProgramUniform1iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec2Array(GLint location, GLsizei count, const GLint* value) {
    glProgramUniform2iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec3Array(GLint location, GLsizei count, const GLint* value) {
    glProgramUniform3iv(program_, location, count, value);
}

void ShaderProgram::setUniformIntVec4Array(GLint location, GLsizei count, const GLint* value) {
    glProgramUniform4iv(program_, location, count, value);
}
#pragma endregion
#pragma region unsigned int arrays
void ShaderProgram::setUniformUintArray(GLint location, GLsizei count, const GLuint* value) {
    glProgramUniform1uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec2Array(GLint location, GLsizei count, const GLuint* value) {
    glProgramUniform2uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec3Array(GLint location, GLsizei count, const GLuint* value) {
    glProgramUniform3uiv(program_, location, count, value);
}

void ShaderProgram::setUniformUintVec4Array(GLint location, GLsizei count, const GLuint* value) {
    glProgramUniform4uiv(program_, location, count, value);
}
#pragma endregion
#pragma region float arrays
void ShaderProgram::setUniformFloatArray(GLint location, GLsizei count, const GLfloat* value) {
    glProgramUniform1fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec2Array(GLint location, GLsizei count, const GLfloat* value) {
    glProgramUniform2fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec3Array(GLint location, GLsizei count, const GLfloat* value) {
    glProgramUniform3fv(program_, location, count, value);
}

void ShaderProgram::setUniformFloatVec4Array(GLint location, GLsizei count, const GLfloat* value) {
    glProgramUniform4fv(program_, location, count, value);
}
#pragma endregion
#pragma region double arrays
void ShaderProgram::setUniformDoubleArray(GLint location, GLsizei count, const GLdouble* value) {
    glProgramUniform1dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec2Array(GLint location, GLsizei count, const GLdouble* value) {
    glProgramUniform2dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec3Array(GLint location, GLsizei count, const GLdouble* value) {
    glProgramUniform3dv(program_, location, count, value);
}

void ShaderProgram::setUniformDoubleVec4Array(GLint location, GLsizei count, const GLdouble* value) {
    glProgramUniform4dv(program_, location, count, value);
}
#pragma endregion
#pragma region float square matrices
void ShaderProgram::setUniformFloatMat2(GLint location, const Matrix<2, 2>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3(GLint location, const Matrix<3, 3>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4(GLint location, const Matrix<4, 4>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4fv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region float non-square matrices
void ShaderProgram::setUniformFloatMat2x3(GLint location, const Matrix<2, 3>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2x3fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3x2(GLint location, const Matrix<3, 2>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3x2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat2x4(GLint location, const Matrix<2, 4>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2x4fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4x2(GLint location, const Matrix<4, 2>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4x2fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat3x4(GLint location, const Matrix<3, 4>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3x4fv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformFloatMat4x3(GLint location, const Matrix<4, 3>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4x3fv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region float square matrix arrays
void ShaderProgram::setUniformFloatMat2Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose) {
    glProgramUniformMatrix2fv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformFloatMat3Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose) {
    glProgramUniformMatrix3fv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformFloatMat4Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose) {
    glProgramUniformMatrix4fv(program_, location, count, transpose, value);
}
#pragma endregion
#pragma region double square matrices
void ShaderProgram::setUniformDoubleMat2(GLint location, const Matrix<2, 2, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3(GLint location, const Matrix<3, 3, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4(GLint location, const Matrix<4, 4, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4dv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region double non-square matrices
void ShaderProgram::setUniformDoubleMat2x3(GLint location, const Matrix<2, 3, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2x3dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3x2(GLint location, const Matrix<3, 2, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3x2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat2x4(GLint location, const Matrix<2, 4, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix2x4dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4x2(GLint location, const Matrix<4, 2, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4x2dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat3x4(GLint location, const Matrix<3, 4, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix3x4dv(program_, location, 1, transpose, matrix);
}

void ShaderProgram::setUniformDoubleMat4x3(GLint location, const Matrix<4, 3, double>& matrix, GLboolean transpose) {
    glProgramUniformMatrix4x3dv(program_, location, 1, transpose, matrix);
}
#pragma endregion
#pragma region double square matrix arrays
void ShaderProgram::setUniformDoubleMat2Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose) {
    glProgramUniformMatrix2dv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformDoubleMat3Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose) {
    glProgramUniformMatrix3dv(program_, location, count, transpose, value);
}

void ShaderProgram::setUniformDoubleMat4Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose) {
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
