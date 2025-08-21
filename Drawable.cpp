#include <iostream>

#include "Common.h"
#include "stb_image.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

Drawable::Drawable(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int shaders, Transform transform) {
    this -> shaders = shaders;
    this -> indicesCount = indices.size();
    this -> verticesCount = vertices.size();

    this -> vertices = vertices;
    this -> indices = indices;
    this -> transform = transform;

    glGenVertexArrays(1, &vertexArrayObject);
    glUseProgram(shaders); // activate shaders and get uniform locations
}

void Drawable::Define(const unsigned int usage, std::string textureFilePath) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindVertexArray(vertexArrayObject);
    buffers = definePrimative(vertices.data(), verticesCount, indices.data(), indicesCount, usage, texture);
}

void Drawable::Draw(const unsigned int mode, mat4x4 view, mat4x4 projection) const {

    // create the model matrix from the transform
    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate(model, transform.position.x, transform.position.y, 0.0f);
    mat4x4_rotate_Z(model, model, transform.rotation);
    mat4x4_scale_aniso(model, model, transform.scale.x, transform.scale.y, 1.0f);

    // combine the matrices into a single MVP matrix
    mat4x4 mvp;
    mat4x4_mul(mvp, view, model);
    mat4x4_mul(mvp, projection, mvp);

    GLint mvpLocation = glGetUniformLocation(shaders, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)mvp);

    glUseProgram(shaders);
    drawPrimative(buffers. indexBuffer, indicesCount, mode, vertexArrayObject, texture);
}

void Drawable::redefineObject(Vertex newVertices[], const int newVerticesCount, unsigned int newIndices[], const int newIndicesCount, unsigned int usage, std::string filePath) {
    this -> verticesCount = newVerticesCount;
    this -> indicesCount = newIndicesCount;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    buffers = definePrimative(newVertices, newVerticesCount, newIndices, newIndicesCount, usage, texture);
}


