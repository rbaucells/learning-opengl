#pragma once

#include <vector>
#include "Math.h"

// structs
struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};

// functions

Buffers definePrimative(Vertex vertices[], const int vertexCount, unsigned int indices[], const int indexCount, unsigned int usage, unsigned int texture);
void drawPrimative(unsigned int indexBuffer, const int indicesCount, unsigned int mode, unsigned int vao, unsigned int texture);

// class Drawable {
// public:
//     unsigned int shader {};
//     unsigned int vertexArrayObject {};
//     std::vector<Vertex> vertices {};
//     std::vector<unsigned int> indices {};
//     int indicesCount {};
//     int verticesCount {};
//
//     Drawable(Vertex vertices[], unsigned int indices[], const int verticesCount, const int indicesCount, unsigned int shader) {
//         this -> vertices.reserve(this -> vertices.size() + verticesCount);
//         std::copy(&vertices[0], &vertices[verticesCount], std::back_inserter(this -> vertices)); // copies raw array into vec
//         this -> indices.reserve(this -> indices.size() + indicesCount);
//         std::copy(&indices[0], &indices[indicesCount], std::back_inserter(this -> indices));
//         this -> shader = shader;
//         this -> indicesCount = indicesCount;
//         this -> verticesCount = verticesCount;
//
//         glGenVertexArrays(1, &vertexArrayObject);
//     }
//
//     void Define(unsigned int usage) {
//         glBindVertexArray(vertexArrayObject);
//         buffers = definePrimative(vertices.data(), verticesCount, indices.data(), indicesCount, usage);
//     }
//
//     /// Make ABSOLUTE SURE that you defined the object previously
//     void Draw(const unsigned int mode) const {
//         glUseProgram(shader);
//         drawPrimative(buffers.indexBuffer, indicesCount, mode, vertexArrayObject);
//     }
//
//     void redefineObject(Vertex newVertices[], const int newVerticesCount, unsigned int newIndices[], const int newIndicesCount, unsigned int usage) {
//         this -> verticesCount = newVerticesCount;
//         this -> indicesCount = newIndicesCount;
//
//         buffers = definePrimative(newVertices, newVerticesCount, newIndices, newIndicesCount, usage);
//     }
//
// private:
//     Buffers buffers = {};
// };

class Drawable {
public:
    unsigned int shaders = 0;
    unsigned int vertexArrayObject = 0;
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    int indicesCount = 0;
    int verticesCount = 0;
    unsigned int texture = 0;

    Drawable(Vertex vertices[], unsigned int indices[], const int verticesCount, const int indicesCount, unsigned int shaders);
    void Define(unsigned int usage, std::string textureFilePath);
    void redefineObject(Vertex newVertices[], int newVerticesCount, unsigned int newIndices[], int newIndicesCount, unsigned int usage, std::string filePath);
    void Draw(unsigned int mode) const;

private:
    Buffers buffers = {};
};