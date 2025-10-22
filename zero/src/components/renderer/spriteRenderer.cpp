#include <iostream>
#include "renderer.h"
#include "../../src/object.h"
#include "../../systems/opengl-wrappers/shader.h"
#include "../../systems/opengl-wrappers/shaderProgram.h"
#include "../../systems/opengl-wrappers/texture.h"
#include "glad/gl.h"

SpriteRenderer::SpriteRenderer(const ComponentParams& params, const Vector2 size, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, int renderingLayer, const Usage usage, const DrawMode drawMode) : RendererBase(params) {
    this->size_ = size;
    this->vertices_ = {
        {{-size_.x / 2, -size_.y / 2}, {0, 0}}, // bottom left
        {{-size_.x / 2, size_.y / 2}, {0, 1}}, // top left
        {{size_.x / 2, -size_.y / 2}, {1, 0}}, // bottom right
        {{size_.x / 2, size_.y / 2}, {1, 1}}, // top right
    };

    this->indices_ = {
        0, 1, 2,
        1, 3, 2
    };
    this->drawMode = drawMode;
    this->shader_ = shader;
    this->renderingLayer_ = renderingLayer;

    this->texture_ = texture;
    this->usage_ = usage;

    addToAllRenderers(renderingLayer);

    glGenVertexArrays(1, &vertexArrayObject_);
    glBindVertexArray(vertexArrayObject_);

    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &vertexBuffer_);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage);
    // define the position vertexAttribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));
    // enable the texture attribute
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &indexBuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices_.size() * sizeof(unsigned int)), indices_.data(), usage);

    // cache uniform locations to avoid lookups in draw
    mvpLocation_ = shader_->getUniformLocation("mvp");
    channelsLocation_ = shader->getUniformLocation("channels");
    alphaLocation_ = shader->getUniformLocation("alpha");
}

SpriteRenderer::SpriteRenderer(const ComponentParams& params, const float pixelsPerUnit, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, const int renderingLayer, const Usage usage, const DrawMode drawMode)
    : SpriteRenderer(params, Vector2(static_cast<float>(texture->getWidth()) / pixelsPerUnit, static_cast<float>(texture->getHeight()) / pixelsPerUnit), texture, shader, renderingLayer, usage, drawMode) {
}

void SpriteRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform->localToWorldMatrix();

    // combine the matrices into a single MVP matrix
    const Matrix<4, 4> mvp = projection * (view * model);

    // make sure were using the shader
    shader_->bind();

    // pass the uniform data using the saved locations
    shader_->setUniformFloatMat4(mvpLocation_, mvp);
    // shader_->setUniformValue(glUniformMatrix4fv, mvpLocation_, 1, GL_FALSE, floatPointer);

    shader_->setUniformInt(channelsLocation_, texture_->getNumberOfChannels());
    // shader_->setUniformValue(glUniform1i, channelsLocation_, texture_->getNumberOfChannels());

    shader_->setUniformFloat(alphaLocation_, alpha);
    // shader_->setUniformValue(glUniform1f, alphaLocation_, alpha);

    onDrawEventDispatcher->invoke(shader_);

    // bind the texture
    texture_->bind();
    // bind the vertexArray
    glBindVertexArray(vertexArrayObject_);
    // make sure were using the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    // draw call
    glDrawElements(drawMode, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}

int SpriteRenderer::getRenderLayer() const {
    return renderingLayer_;
}

void SpriteRenderer::setRenderLayer(const int layer) {
    this->renderingLayer_ = layer;

    addToAllRenderers(renderingLayer_);
}

void SpriteRenderer::changeSize(const Vector2 size, const Usage usage) {
    this->size_ = size;
    this->usage_ = usage;

    this->vertices_ = {
        {{-size_.x / 2, -size_.y / 2}, {0, 0}}, // bottom left
        {{-size_.x / 2, size_.y / 2}, {0, 1}}, // top left
        {{size_.x / 2, -size_.y / 2}, {1, 0}}, // bottom right
        {{size_.x / 2, size_.y / 2}, {1, 1}}, // top right
    };

    glBindVertexArray(vertexArrayObject_);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage_);
}

void SpriteRenderer::changeSize(const float pixelsPerUnit, const Usage usage) {
    changeSize(Vector2(static_cast<float>(texture_->getWidth()) / pixelsPerUnit, static_cast<float>(texture_->getHeight()) / pixelsPerUnit), usage);
}

void SpriteRenderer::changeTexture(const std::shared_ptr<Texture>& texture, const Usage usage) {
    this->texture_ = texture;
    this->usage_ = usage;
}

void SpriteRenderer::changeShader(const std::shared_ptr<ShaderProgram>& shader) {
    this->shader_ = shader;
}

void SpriteRenderer::changeUsage(const Usage usage) {
    this->usage_ = usage;

    glBindVertexArray(vertexArrayObject_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage_);
}

std::weak_ptr<Tween<float>> SpriteRenderer::alphaTween(float start, float end, float duration, const Curve& curve) {
    return addTween(std::make_shared<Tween<float>>(&alpha, start, end, duration, curve));
}

std::weak_ptr<Tween<float>> SpriteRenderer::alphaTween(float end, float duration, const Curve& curve) {
    return addTween(std::make_shared<Tween<float>>(&alpha, alpha, end, duration, curve));
}