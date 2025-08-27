#pragma once
#include "component.h"

class Renderer : public Component {

public:
    Renderer(Object *owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int usage, const std::string& texturePath, const bool flipTexture, const int textureParam, unsigned int shaderProgram);
    void Draw(const ColumnMatrix4x4& view, const ColumnMatrix4x4 &projection, int mode) const;

    float alpha = 1.f;
private:
    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};
    Buffers buffers {};
    unsigned int shaderProgram = 0;
    unsigned int vao = 0;
    unsigned int texture = 0;

    int numberOfChannels = 0;
};
