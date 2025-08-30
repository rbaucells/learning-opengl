#pragma once
#include <map>

#include "component.h"

class Renderer : public Component {
public:
    Renderer(Object *owner, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int usage, const std::string &texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);

    void Draw(const ColumnMatrix4x4 &view, const ColumnMatrix4x4 &projection, int mode) const;

    float alpha = 1.f;
    unsigned int layer = 0;

    ~Renderer() override;

private:
    std::vector<Vertex> vertices{};
    std::vector<unsigned int> indices{};
    Buffers buffers{};
    int numberOfChannels = 0;
    unsigned int shaderProgram = 0;
    unsigned int vao = 0;
    unsigned int texture = 0;

    int mvpLocation = -1;
    int channelsLocation = -1;
    int alphaLocation = -1;
};

inline std::map<unsigned int, std::vector<Renderer*>> allRenderers;
