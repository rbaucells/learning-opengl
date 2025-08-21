#include "Common.h"
#include "stb_image.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

Drawable::Drawable(Vertex vertices[], unsigned int indices[], const int verticesCount, const int indicesCount, unsigned int shaders) {
    this -> vertices.reserve(this -> vertices.size() + verticesCount);
    std::copy(&vertices[0], &vertices[verticesCount], std::back_inserter(this -> vertices)); // copies raw array into vec
    this -> indices.reserve(this -> indices.size() + indicesCount);
    std::copy(&indices[0], &indices[indicesCount], std::back_inserter(this -> indices));
    this -> shaders = shaders;
    this -> indicesCount = indicesCount;
    this -> verticesCount = verticesCount;

    glGenVertexArrays(1, &vertexArrayObject);
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

void Drawable::Draw(const unsigned int mode) const {
    glUseProgram(shaders);
    drawPrimative(buffers.indexBuffer, indicesCount, mode, vertexArrayObject, texture);
}

void Drawable::redefineObject(Vertex newVertices[], const int newVerticesCount, unsigned int newIndices[], const int newIndicesCount, unsigned int usage, std::string filePath) {
    this -> verticesCount = newVerticesCount;
    this -> indicesCount = newIndicesCount;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    buffers = definePrimative(newVertices, newVerticesCount, newIndices, newIndicesCount, usage, texture);
}


