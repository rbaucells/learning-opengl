#pragma once
#include "component.h"

class Renderer : public Component {

public:
    Renderer(Object *owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int usage, const std::string& texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);
    void Draw(const ColumnMatrix4x4& view, const ColumnMatrix4x4 &projection, int mode) const;

    float alpha = 1.f;
    int layer;
private:
    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};
    Buffers buffers {};
    unsigned int shaderProgram = 0;
    unsigned int vao = 0;
    unsigned int texture = 0;

    int numberOfChannels = 0;
};
