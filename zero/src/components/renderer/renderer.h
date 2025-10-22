#pragma once
#include <map>
#include "glad/gl.h"
#include "../../systems/component.h"
#include "math++/math.h"

class ShaderProgram;
class Texture;

class RendererBase : public Component {
public:
    // we use the constructor from Component
    using Component::Component;

    virtual void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const = 0;

    // std::weak_ptr<Shader> = Shader Being Used
    std::shared_ptr<Publisher<std::weak_ptr<ShaderProgram>>> onDrawEventDispatcher = Publisher<std::weak_ptr<ShaderProgram>>::create();

    virtual std::weak_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve) = 0;
    virtual std::weak_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve) = 0;

    ~RendererBase() override = default;

    enum DrawMode : int {
        points = GL_POINTS,
        line_strip = GL_LINE_STRIP,
        line_loop = GL_LINE_LOOP,
        lines = GL_LINES,
        line_strip_adjacency = GL_LINE_STRIP_ADJACENCY,
        lines_adjacency = GL_LINES_ADJACENCY,
        triangle_strip = GL_TRIANGLE_STRIP,
        triangle_fan = GL_TRIANGLE_FAN,
        triangles = GL_TRIANGLES,
        triangle_strip_adjacancy = GL_TRIANGLE_STRIP_ADJACENCY,
        triangles_adjacency = GL_TRIANGLES_ADJACENCY,
        patches = GL_PATCHES
    };

    // Performance hints for OpenGl, do not modify how its used
    enum Usage : GLuint {
        // The data store contents will be specified once by the application, and sourced many times
        static_draw = GL_STATIC_DRAW,
        // The data store contents will be respecified repeatedly by the application, and sourced many times.
        dynamic_draw = GL_DYNAMIC_DRAW,
        // The data store contents will be specified once by the application, and sourced at most a few times.
        stream_draw = GL_STREAM_DRAW
    };

protected:
    void addToAllRenderers(int layer);

    int renderingLayer_ = 0;

    Subscription<const Matrix<4, 4>&, const Matrix<4, 4>&> drawSubscription_;
};

class SpriteSheetRenderer final : public RendererBase {
public:
    SpriteSheetRenderer(const ComponentParams& params, Vector2 size, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, Usage usage = dynamic_draw, DrawMode drawMode = triangles);
    SpriteSheetRenderer(const ComponentParams& params, float pixelsPerUnit, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, Usage usage = dynamic_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void changeSize(Vector2 size, Usage usage = dynamic_draw);
    void changeSize(float pixelsPerUnit, Usage usage = dynamic_draw);

    void changeTexture(const std::shared_ptr<Texture>& texture, int gridWidth, int gridHeight, int padding, Usage usage = dynamic_draw);

    void changeShader(const std::shared_ptr<ShaderProgram>& shader);

    void changeUsage(Usage usage);

    std::weak_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve) override;
    std::weak_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve) override;

    void moveTo(int i);
    void moveToNext();

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

    std::shared_ptr<ShaderProgram> shader_ = nullptr;

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
    SpriteRenderer(const ComponentParams& params, Vector2 size, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);
    SpriteRenderer(const ComponentParams& params, float pixelsPerUnit, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void changeSize(Vector2 size, Usage usage = static_draw);
    void changeSize(float pixelsPerUnit, Usage usage = static_draw);

    void changeTexture(const std::shared_ptr<Texture>& texture, Usage usage = static_draw);

    void changeShader(const std::shared_ptr<ShaderProgram>& shader);

    void changeUsage(Usage usage);

    std::weak_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve) override;
    std::weak_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve) override;

    float alpha = 1;
    DrawMode drawMode = triangles;

private:
    std::shared_ptr<Texture> texture_ = nullptr;
    std::shared_ptr<ShaderProgram> shader_ = nullptr;

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
    CustomRenderer(const ComponentParams& params, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, Usage usage = static_draw, DrawMode drawMode = triangles);

    void draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const override;

    [[nodiscard]] int getRenderLayer() const;
    void setRenderLayer(int layer);

    void redefine(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, Usage usage);

    void changeUsage(Usage usage) const;

    std::weak_ptr<Tween<float>> alphaTween(float start, float end, float duration, const Curve& curve) override;
    std::weak_ptr<Tween<float>> alphaTween(float end, float duration, const Curve& curve) override;

    float alpha = 1.f;
    DrawMode drawMode = triangles;

private:
    std::shared_ptr<Texture> texture_ = nullptr;
    std::shared_ptr<ShaderProgram> shader_ = nullptr;

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

inline std::map<unsigned int, std::shared_ptr<Publisher<const Matrix<4, 4>&, const Matrix<4, 4>&>>> renderersDrawPublishers;
