#pragma once
#include <map>
#include "../../systems/component.h"
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

class SpriteSheetRenderer final : public RendererBase {
public:
    SpriteSheetRenderer(Object* owner, Vector2 size, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = dynamic_draw, DrawMode drawMode = triangles);
    SpriteSheetRenderer(Object* owner, float pixelsPerUnit, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = dynamic_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] Bounds getBounds() const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void changeSize(Vector2 size, Usage usage = dynamic_draw);
    void changeSize(float pixelsPerUnit, Usage usage = dynamic_draw);

    void changeTexture(const std::shared_ptr<Texture>& texture, int gridWidth, int gridHeight, int padding, Usage usage = dynamic_draw);

    void changeShader(const std::shared_ptr<Shader>& shader);

    void changeUsage(Usage usage);

    std::unique_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve = Curve::linear);
    std::unique_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve = Curve::linear);

    void moveTo(int i);
    void moveToNext();

    ~SpriteSheetRenderer() override;

    float alpha = 1;
    DrawMode drawMode = triangles;

private:
    // texture info
    std::shared_ptr<Texture> texture_ = nullptr;

    int imageWidth_ = 0;
    int imageHeight_ = 0;

    int gridWidth_ = 0;
    int gridHeight_ = 0;
    int padding_ = 0;

    int numberOfColumns_ = 0;
    int numberOfRows_ = 0;

    int currentIndex_ = 1;

    Usage usage_ = static_draw;

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

    Vector2 size_ = {};
};

class SpriteRenderer final : public RendererBase {
public:
    SpriteRenderer(Object* owner, Vector2 size, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);
    SpriteRenderer(Object* owner, float pixelsPerUnit, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] Bounds getBounds() const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void changeSize(Vector2 size, Usage usage = static_draw);
    void changeSize(float pixelsPerUnit, Usage usage = static_draw);

    void changeTexture(const std::shared_ptr<Texture>& texture, Usage usage = static_draw);

    void changeShader(const std::shared_ptr<Shader>& shader);

    void changeUsage(Usage usage);

    std::unique_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve = Curve::linear);
    std::unique_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve = Curve::linear);

    ~SpriteRenderer() override;

    float alpha = 1;
    DrawMode drawMode = triangles;

private:
    std::shared_ptr<Texture> texture_ = nullptr;
    std::shared_ptr<Shader> shader_ = nullptr;

    Usage usage_;

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

    Vector2 size_ = {};
};

class CustomRenderer final : public RendererBase {
public:
    CustomRenderer(Object* owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] Bounds getBounds() const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void redefine(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, Usage usage);

    void changeUsage(Usage usage) const;

    std::unique_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve = Curve::linear);
    std::unique_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve = Curve::linear);

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
