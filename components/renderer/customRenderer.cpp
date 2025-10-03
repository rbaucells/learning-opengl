#include <iostream>
#include "renderer.h"
#include "../../object.h"
#include "../../math/vertex.h"
#include "../../systems/opengl wrappers/shader.h"
#include "../../systems/opengl wrappers/shaderProgram.h"
#include "../../systems/opengl wrappers/texture.h"
#include "glad/gl.h"

CustomRenderer::CustomRenderer(Object* owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, const int renderingLayer, const Usage usage, const DrawMode drawMode) : RendererBase(owner) {
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->shader_ = shader;
    this->renderingLayer_ = renderingLayer;
    this->drawMode = drawMode;

    this->texture_ = texture;

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

void CustomRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform.localToWorldMatrix();

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

Bounds CustomRenderer::getBounds() const {
    Bounds bounds = {};

    Vertex maxX;
    Vertex minX;

    Vertex maxY;
    Vertex minY;

    for (const auto& [position, uv] : vertices_) {
        if (position.x > maxX.position.x) {
            maxX.position.x = position.x;
        }
        else if (position.x < minX.position.x) {
            minX.position.x = position.x;
        }

        if (position.y > maxY.position.y) {
            maxY.position.y = position.y;
        }
        else if (position.y < minY.position.y) {
            minY.position.y = position.y;
        }
    }

    bounds.center = object->transform.getGlobalPosition();

    bounds.max = {maxX.position.x / 2.f, maxY.position.y / 2.f};
    bounds.min = {minX.position.x / 2.f, minY.position.y / 2.f};

    bounds.size = bounds.max - bounds.min;

    return bounds;
}

int CustomRenderer::getRenderLayer() const {
    return renderingLayer_;
}

void CustomRenderer::setRenderLayer(const int layer) {
    this->renderingLayer_ = layer;

    addToAllRenderers(renderingLayer_);
}

void CustomRenderer::redefine(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture, const std::shared_ptr<ShaderProgram>& shader, const Usage usage) {
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->shader_ = shader;
    this->texture_ = texture;

    glBindVertexArray(vertexArrayObject_);

    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices_.size() * sizeof(unsigned int)), indices_.data(), usage);
}

void CustomRenderer::changeUsage(const Usage usage) const {
    glBindVertexArray(vertexArrayObject_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage);
}

std::weak_ptr<Tween<float>> CustomRenderer::alphaTween(float start, float end, float duration, const Curve& curve) {
    return addTween(std::make_shared<Tween<float>>(&alpha, start, end, duration, curve));
}

std::weak_ptr<Tween<float>> CustomRenderer::alphaTween(float end, float duration, const Curve& curve) {
    return addTween(std::make_shared<Tween<float>>(&alpha, alpha, end, duration, curve));
}