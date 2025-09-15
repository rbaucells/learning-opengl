#pragma once
#include <map>
#include "../../systems/component.h"
#include "../../math/buffers.h"
#include "../../math/matrix.h"
#include "../../math/vertex.h"

class RendererBase : public Component {
public:
    using Component::Component;
    ~RendererBase() override = default;

    virtual void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection, int mode) const = 0;

    virtual void changeSpriteTexture(const std::string& texturePath, bool flipTexture, int textureParam) {};
};

class SpriteSheetRenderer final : public RendererBase {
public:
    SpriteSheetRenderer(Object* owner, int gridWitdh, int gridHeight, int padding, unsigned int usage, const std::string& texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection, int mode) const override;

    void moveTo(int i);

    ~SpriteSheetRenderer() override;

    void changeSpriteTexture(const std::string& texturePath, bool flipTexture, int textureParam) override;

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};

    int numberOfChannels_ = 0;

    unsigned int shaderProgram_ = 0;
    unsigned int vao_ = 0;
    unsigned int texture_ = 0;

    int mvpLocation_ = -1;
    int channelsLocation_ = -1;
    int alphaLocation_ = -1;

    unsigned int layer_ = 0;
    float alpha_ = 1.f;

    int numberOfColumns_ = 0;
    int numberOfRows_ = 0;

    int gridWitdh_ = 0;
    int gridHeight_ = 0;
    int padding_ = 0;

    int imageWidth_ = 0;
    int imageHeight_ = 0;

    unsigned int usage_ = 0;
};

class SpriteRenderer final : public RendererBase {
public:
    SpriteRenderer(Object* owner, Vector2 size, unsigned int usage, const std::string& texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection, int mode) const override;

    ~SpriteRenderer() override;

    void changeSpriteTexture(const std::string& texturePath, bool flipTexture, int textureParam) override;

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};
    int numberOfChannels_ = 0;
    unsigned int shaderProgram_ = 0;
    unsigned int vao_ = 0;
    unsigned int texture_ = 0;

    int mvpLocation_ = -1;
    int channelsLocation_ = -1;
    int alphaLocation_ = -1;

    unsigned int layer_ = 0;
    float alpha_ = 1.f;
};

// same thing
typedef SpriteRenderer SimpleRenderer;

class CustomRenderer final : public RendererBase {
public:
    CustomRenderer(Object* owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int usage, const std::string& texturePath, bool flipTexture, int textureParam, unsigned int shaderProgram, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection, int mode) const override;

    ~CustomRenderer() override;

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};
    int numberOfChannels_ = 0;
    unsigned int shaderProgram_ = 0;
    unsigned int vao_ = 0;
    unsigned int texture_ = 0;

    int mvpLocation_ = -1;
    int channelsLocation_ = -1;
    int alphaLocation_ = -1;

    unsigned int layer_ = 0;
    float alpha_ = 1.f;
};

// same thing
typedef CustomRenderer ComplexRenderer;

inline std::map<unsigned int, std::vector<RendererBase*>> allRenderers;
