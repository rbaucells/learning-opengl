#pragma once
#include <map>
#include "../../systems/component.h"
#include "../../math/buffers.h"
#include "../../math/matrix.h"
#include "../../math/vertex.h"
#include "glad/gl.h"

class Shader;
class Texture;

class RendererBase : public Component {
public:
    using Component::Component;

    ~RendererBase() override = default;

    virtual void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const = 0;
};

class SpriteSheetRenderer final : public RendererBase {
public:
    SpriteSheetRenderer(Object* owner, int gridWitdh, int gridHeight, int padding, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    void moveTo(int i);

    void setDrawMode(int mode);

    ~SpriteSheetRenderer() override;

    void changeSpriteTexture(int gridWitdh, int gridHeight, int padding, const std::shared_ptr<Texture>& texture);

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};

    std::shared_ptr<Texture> texture_;
    unsigned int vao_ = 0;

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

    std::shared_ptr<Shader> shader_;

    int drawMode_ = GL_TRIANGLES;
};

class SpriteRenderer final : public RendererBase {
public:
    SpriteRenderer(Object* owner, Vector2 size, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    void setDrawMode(int mode);

    ~SpriteRenderer() override;

    void changeSpriteTexture(const std::shared_ptr<Texture>& texture);

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};

    std::shared_ptr<Shader> shader_;
    unsigned int vao_ = 0;
    std::shared_ptr<Texture> texture_;

    int mvpLocation_ = -1;
    int channelsLocation_ = -1;
    int alphaLocation_ = -1;

    unsigned int layer_ = 0;
    float alpha_ = 1.f;

    int drawMode_ = GL_TRIANGLES;
};

// same thing
typedef SpriteRenderer SimpleRenderer;

class CustomRenderer final : public RendererBase {
public:
    CustomRenderer(Object* owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    void setDrawMode(int mode);

    ~CustomRenderer() override;

private:
    std::vector<Vertex> vertices_ {};
    std::vector<unsigned int> indices_ {};
    Buffers buffers_ {};

    std::shared_ptr<Shader> shader_;
    unsigned int vao_ = 0;

    int mvpLocation_ = -1;
    int channelsLocation_ = -1;
    int alphaLocation_ = -1;

    unsigned int layer_ = 0;
    float alpha_ = 1.f;

    std::shared_ptr<Texture> texture_;

    int drawMode_ = GL_TRIANGLES;
};

// same thing
typedef CustomRenderer ComplexRenderer;

inline std::map<unsigned int, std::vector<RendererBase*>> allRenderers;
