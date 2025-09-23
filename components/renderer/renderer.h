#pragma once
#include <map>
#include "../../systems/component.h"
#include "../../math/buffers.h"
#include "../../math/matrix.h"
#include "../../math/vertex.h"
#include "glad/gl.h"
#include "../../math/bounds.h"

class Shader;
class Texture;

class RendererBase : public Component {
public:
    // we use the constructor from Component
    using Component::Component;

    virtual void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const = 0;

    [[nodiscard]] virtual Bounds getBounds() const = 0;

    ~RendererBase() override = default;

    enum DrawMode : int {
        triangles = GL_TRIANGLES,
        triangle_strip = GL_TRIANGLE_STRIP,
        triangle_fan = GL_TRIANGLE_FAN,
        lines = GL_LINES,
        line_strip = GL_LINE_STRIP,
        line_loop = GL_LINE_LOOP
    };

    enum Usage : GLuint {
        static_draw = GL_STATIC_DRAW,
        dynamic_draw = GL_DYNAMIC_DRAW,
    };

protected:
    void removeFromAllRenderers();
    void addToAllRenderers(int layer);

    int renderingLayer_ = 0;
};

// class SpriteSheetRenderer final : public RendererBase {
// public:
//     DrawMode drawMode = triangles;
//
//     SpriteSheetRenderer(Object* owner, Vector2 size, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer, Usage usage = static_draw, DrawMode drawMode = triangles);
//
//     void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;
//
//     [[nodiscard]] Bounds getBounds() const override;
//
//     ~SpriteSheetRenderer() override;
//
// private:
//     std::vector<Vertex> vertices_ = {};
//     std::vector<unsigned int> indices_ = {};
//
//     GLuint vertexBuffer_ = 0;
//     GLuint indexBuffer_ = 0;
//
//     std::shared_ptr<Texture> texture_ = nullptr;
//     std::shared_ptr<Shader> shader_ = nullptr;
//
//     GLuint vertexArrayObject_ = 0;
//
//     // uniforms
//     GLint mvpLocation_ = -1;
//     GLint channelsLocation_ = -1;
//     GLint alphaLocation_ = -1;
//
//     int layer_ = 0;
//     float alpha_ = 1;
//
//     Usage usage_ = static_draw;
//
//     Vector2 size_;
// };

// class SpriteRenderer final : public RendererBase {
// public:
//     SpriteRenderer(Object* owner, Vector2 size, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer);
//     SpriteRenderer(Object* owner, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer, float pixelsPerUnit = 100);
//
//     void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;
//
//     [[nodiscard]] Bounds getBounds() const override;
//
//     ~SpriteRenderer() override;
//
// private:
//     std::vector<Vertex> vertices_{};
//     std::vector<unsigned int> indices_{};
//     Buffers buffers_{};
//
//     std::shared_ptr<Shader> shader_;
//     unsigned int vao_ = 0;
//     std::shared_ptr<Texture> texture_;
//
//     int mvpLocation_ = -1;
//     int channelsLocation_ = -1;
//     int alphaLocation_ = -1;
//
//     unsigned int layer_ = 0;
//     float alpha_ = 1.f;
//
//     int drawMode_ = GL_TRIANGLES;
// };

class CustomRenderer final : public RendererBase {
public:
    CustomRenderer(Object* owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] Bounds getBounds() const override;

    int getRenderLayer() const;
    void setRenderLayer(int layer);

    ~CustomRenderer() override;

    float alpha = 1.f;
    DrawMode drawMode = triangles;

private:
    std::shared_ptr<Texture> texture_ = nullptr;
    std::shared_ptr<Shader> shader_ = nullptr;

    // uniforms locations
    GLint mvpLocation_ = -1;
    GLint channelsLocation_ = -1;
    GLint alphaLocation_ = -1;

    // opengl objects
    GLuint vertexArrayObject_ = 0;

    GLuint vertexBuffer_ = 0;
    GLuint indexBuffer_ = 0;

    // object data
    std::vector<Vertex> vertices_ = {};
    std::vector<unsigned int> indices_ = {};
};

inline std::map<unsigned int, std::vector<RendererBase*>> allRenderers;
