#pragma once
#include <map>
#include "../math/mathematics.h"
#include "../systems/component.h"

class Renderer final : public Component {
public:
    Renderer(Object *owner, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int usage, const std::string &texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4> &projection, int mode) const;

    void tweenAlpha(float target, float duration, Easer easer);

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

    unsigned int layer = 0;
    float alpha = 1.f;
};

inline std::map<unsigned int, std::vector<Renderer*>> allRenderers;
