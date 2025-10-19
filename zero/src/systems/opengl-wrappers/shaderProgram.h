#pragma once
#include <sstream>
#include <string>
#include "shader.h"
#include "math++/math.h"
#include "glad/gl.h"
#include <vector>

class Shader;

class ShaderProgram {
public:
    static std::shared_ptr<ShaderProgram> create();

    static std::shared_ptr<ShaderProgram> create(std::initializer_list<std::shared_ptr<Shader>> shaders);

    /**
     * @brief Attaches the Shader object to the program
     * @param shader The shader to attach. Once attached, shader can be safetely deleted
     * @note After shader is attached, it can be deleted safetely
     */
    void attach(const std::shared_ptr<Shader>& shader);

    /**
     * @brief Detaches the Shader object from the program
     * @param shader The shader to detach.
     * @note If the shader object is not being used in any other programs, the
     */
    void detach(const std::shared_ptr<Shader>& shader);

    void linkAndVerify() const;

    void bind() const;

    [[nodiscard]] int getUniformLocation(const std::string& name) const;

#pragma region setters
#pragma region ints
    void setUniformInt(GLint location, GLint v0) const;
    void setUniformInt(GLint location, GLint v0, GLint v1) const;
    void setUniformInt(GLint location, GLint v0, GLint v1, GLint v2) const;
    void setUniformInt(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) const;
#pragma endregion
#pragma region unsinged ints
    void setUniformUint(GLint location, GLuint v0) const;
    void setUniformUint(GLint location, GLuint v0, GLuint v1) const;
    void setUniformUint(GLint location, GLuint v0, GLuint v1, GLuint v2) const;
    void setUniformUint(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const;
#pragma endregion
#pragma region floats
    void setUniformFloat(GLint location, GLfloat v0) const;
    void setUniformFloat(GLint location, GLfloat v0, GLfloat v1) const;
    void setUniformFloat(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const;
    void setUniformFloat(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
#pragma endregion
#pragma region doubles
    void setUniformDouble(GLint location, GLdouble v0) const;
    void setUniformDouble(GLint location, GLdouble v0, GLdouble v1) const;
    void setUniformDouble(GLint location, GLdouble v0, GLdouble v1, GLdouble v2) const;
    void setUniformDouble(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const;
#pragma endregion
#pragma region int arrays
    void setUniformIntArray(GLint location, GLsizei count, const GLint* value) const;
    void setUniformIntVec2Array(GLint location, GLsizei count, const GLint* value) const;
    void setUniformIntVec3Array(GLint location, GLsizei count, const GLint* value) const;
    void setUniformIntVec4Array(GLint location, GLsizei count, const GLint* value) const;
#pragma endregion
#pragma region unsinged int arrays
    void setUniformUintArray(GLint location, GLsizei count, const GLuint* value) const;
    void setUniformUintVec2Array(GLint location, GLsizei count, const GLuint* value) const;
    void setUniformUintVec3Array(GLint location, GLsizei count, const GLuint* value) const;
    void setUniformUintVec4Array(GLint location, GLsizei count, const GLuint* value) const;
#pragma endregion
#pragma region float arrays
    void setUniformFloatArray(GLint location, GLsizei count, const GLfloat* value) const;
    void setUniformFloatVec2Array(GLint location, GLsizei count, const GLfloat* value) const;
    void setUniformFloatVec3Array(GLint location, GLsizei count, const GLfloat* value) const;
    void setUniformFloatVec4Array(GLint location, GLsizei count, const GLfloat* value) const;
#pragma endregion
#pragma region double arrays
    void setUniformDoubleArray(GLint location, GLsizei count, const GLdouble* value) const;
    void setUniformDoubleVec2Array(GLint location, GLsizei count, const GLdouble* value) const;
    void setUniformDoubleVec3Array(GLint location, GLsizei count, const GLdouble* value) const;
    void setUniformDoubleVec4Array(GLint location, GLsizei count, const GLdouble* value) const;
#pragma endregion
#pragma region float square matrixes
    void setUniformFloatMat2(GLint location, const Matrix<2, 2>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat3(GLint location, const Matrix<3, 3>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat4(GLint location, const Matrix<4, 4>& matrix, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma region float non-square matrixes
    void setUniformFloatMat2x3(GLint location, const Matrix<2, 3>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat3x2(GLint location, const Matrix<3, 2>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat2x4(GLint location, const Matrix<2, 4>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat4x2(GLint location, const Matrix<4, 2>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat3x4(GLint location, const Matrix<3, 4>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat4x3(GLint location, const Matrix<4, 3>& matrix, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma region float square matrix arrays
    void setUniformFloatMat2Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat3Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose = GL_FALSE) const;
    void setUniformFloatMat4Array(GLint location, GLsizei count, const GLfloat* value, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma region double sqaure matrixes
    void setUniformDoubleMat2(GLint location, const Matrix<2, 2, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat3(GLint location, const Matrix<3, 3, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat4(GLint location, const Matrix<4, 4, double>& matrix, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma region double non-square matrixes
    void setUniformDoubleMat2x3(GLint location, const Matrix<2, 3, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat3x2(GLint location, const Matrix<3, 2, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat2x4(GLint location, const Matrix<2, 4, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat4x2(GLint location, const Matrix<4, 2, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat3x4(GLint location, const Matrix<3, 4, double>& matrix, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat4x3(GLint location, const Matrix<4, 3, double>& matrix, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma region double square matrix arrays
    void setUniformDoubleMat2Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat3Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose = GL_FALSE) const;
    void setUniformDoubleMat4Array(GLint location, GLsizei count, const GLdouble* value, GLboolean transpose = GL_FALSE) const;
#pragma endregion
#pragma endregion
#pragma region getters
#pragma region ints
    [[nodiscard]] GLint getUniformInt(GLint location) const;
    void getUniformInt(GLint location, GLint* data) const;
#pragma endregion
#pragma region unsigned ints
    [[nodiscard]] GLuint getUniformUint(GLint location) const;
    void getUniformUint(GLint location, GLuint* data) const;
#pragma endregion
#pragma region floats
    [[nodiscard]] GLfloat getUniformFloat(GLint location) const;
    void getUniformFloat(GLint location, GLfloat* data) const;
#pragma endregion
#pragma region doubles
    [[nodiscard]] GLdouble getUniformDouble(GLint location) const;
    void getUniformDouble(GLint location, GLdouble* data) const;
#pragma endregion
#pragma region int arrays
    void getUniformIntArray(GLint location, GLint* data) const; // Reads count * 1
    void getUniformIntVec2Array(GLint location, GLint* data) const; // Reads count * 2
    void getUniformIntVec3Array(GLint location, GLint* data) const; // Reads count * 3
    void getUniformIntVec4Array(GLint location, GLint* data) const; // Reads count * 4
#pragma endregion
#pragma region unsinged int arrays
    void getUniformUintArray(GLint location, GLuint* data) const; // Reads count * 1
    void getUniformUintVec2Array(GLint location, GLuint* data) const; // Reads count * 2
    void getUniformUintVec3Array(GLint location, GLuint* data) const; // Reads count * 3
    void getUniformUintVec4Array(GLint location, GLuint* data) const; // Reads count * 4
#pragma endregion
#pragma region float arrays
    void getUniformFloatArray(GLint location, GLfloat* data) const; // Reads count * 1
    void getUniformFloatVec2Array(GLint location, GLfloat* data) const; // Reads count * 2
    void getUniformFloatVec3Array(GLint location, GLfloat* data) const; // Reads count * 3
    void getUniformFloatVec4Array(GLint location, GLfloat* data) const; // Reads count * 4
#pragma endregion
#pragma region double arrays
    void getUniformDoubleArray(GLint location, GLdouble* data) const; // Reads count * 1
    void getUniformDoubleVec2Array(GLint location, GLdouble* data) const; // Reads count * 2
    void getUniformDoubleVec3Array(GLint location, GLdouble* data) const; // Reads count * 3
    void getUniformDoubleVec4Array(GLint location, GLdouble* data) const; // Reads count * 4
#pragma endregion
#pragma region float square matrixes
    [[nodiscard]] Matrix<2, 2> getUniformFloatMat2(GLint location) const;
    [[nodiscard]] Matrix<3, 3> getUniformFloatMat3(GLint location) const;
    [[nodiscard]] Matrix<4, 4> getUniformFloatMat4(GLint location) const;
#pragma endregion
#pragma region float non-square matrixes
    [[nodiscard]] Matrix<2, 3> getUniformFloatMat2x3(GLint location) const;
    [[nodiscard]] Matrix<3, 2> getUniformFloatMat3x2(GLint location) const;
    [[nodiscard]] Matrix<2, 4> getUniformFloatMat2x4(GLint location) const;
    [[nodiscard]] Matrix<4, 2> getUniformFloatMat4x2(GLint location) const;
    [[nodiscard]] Matrix<3, 4> getUniformFloatMat3x4(GLint location) const;
    [[nodiscard]] Matrix<4, 3> getUniformFloatMat4x3(GLint location) const;
#pragma endregion
#pragma region float sqaure matrix arrays
    void getUniformFloatMat2Array(GLint location, GLfloat* value) const;
    void getUniformFloatMat3Array(GLint location, GLfloat* value) const;
    void getUniformFloatMat4Array(GLint location, GLfloat* value) const;
#pragma endregion
#pragma region double square matrixes
    [[nodiscard]] Matrix<2, 2, double> getUniformDoubleMat2(GLint location) const;
    [[nodiscard]] Matrix<3, 3, double> getUniformDoubleMat3(GLint location) const;
    [[nodiscard]] Matrix<4, 4, double> getUniformDoubleMat4(GLint location) const;
#pragma endregion
#pragma region double non-square matrixes
    [[nodiscard]] Matrix<2, 3, double> getUniformDoubleMat2x3(GLint location) const;
    [[nodiscard]] Matrix<3, 2, double> getUniformDoubleMat3x2(GLint location) const;
    [[nodiscard]] Matrix<2, 4, double> getUniformDoubleMat2x4(GLint location) const;
    [[nodiscard]] Matrix<4, 2, double> getUniformDoubleMat4x2(GLint location) const;
    [[nodiscard]] Matrix<3, 4, double> getUniformDoubleMat3x4(GLint location) const;
    [[nodiscard]] Matrix<4, 3, double> getUniformDoubleMat4x3(GLint location) const;
#pragma endregion
#pragma region double sqaure matrix arrays
    void getUniformDoubleMat2Array(GLint location, GLdouble* value) const;
    void getUniformDoubleMat3Array(GLint location, GLdouble* value) const;
    void getUniformDoubleMat4Array(GLint location, GLdouble* value) const;
#pragma endregion
#pragma endregion

    ~ShaderProgram();

private:
    GLuint program_;

    ShaderProgram();

    std::vector<std::shared_ptr<Shader>> shaderObjects_;
};
