#include <iostream>
#include "renderer.h"
#include "../../object.h"
#include "glad/gl.h"
#include "../../math/vertex.h"
#include "../../systems/opengl wrappers/shader.h"
#include "../../systems/opengl wrappers/texture.h"

SpriteSheetRenderer::SpriteSheetRenderer(Object* owner, const int gridWitdh, const int gridHeight, const int padding, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer) : RendererBase(owner) {
    indices_ = {
        0, 1, 2,
        1, 3, 2
    };

    this->texture_ = texture;
    this->gridWitdh_ = gridWitdh;
    this->gridHeight_ = gridHeight;

    this->imageHeight_ = texture_->getHeight();
    this->imageWidth_ = texture_->getWidth();

    this->usage_ = usage;
    this->padding_ = padding;

    this->shader_ = shader;
    this->layer_ = layer;

    if (const auto it = allRenderers.find(layer); it != allRenderers.end()) {
        // the layer already exists
        it->second.push_back(this);
    }
    else {
        // the layer doesnt exist
        allRenderers[layer] = {this};
    }

    // simple division while rounding down to ensure no cropped out things
    this->numberOfColumns_ = imageWidth_ / gridWitdh_;
    this->numberOfRows_ = imageHeight_ / gridHeight_;

    texture_->bind();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // buffer id
    unsigned int vertexBuffer;
    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &vertexBuffer);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    constexpr int leftX = 0;
    constexpr int bottomY = 0;

    const float topY = static_cast<float>(gridHeight_) / imageHeight_;
    const float rightX = static_cast<float>(gridWitdh_) / imageWidth_;

    vertices_ = {
        {Vector2(static_cast<float>(-gridWitdh_) / 2, static_cast<float>(-gridHeight_) / 2), Vector2(leftX, bottomY)}, // bottom left
        {Vector2(static_cast<float>(-gridWitdh_) / 2, static_cast<float>(gridHeight_) / 2), Vector2(leftX, topY)}, // top left
        {Vector2(static_cast<float>(gridWitdh_) / 2, static_cast<float>(-gridHeight_) / 2), Vector2(rightX, bottomY)}, // bottom right
        {Vector2(static_cast<float>(gridWitdh_) / 2, static_cast<float>(gridHeight_) / 2), Vector2(rightX, topY)}, // top right
    };

    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), usage);
    // define the position vertexAttribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));
    // enable the texture attribute
    glEnableVertexAttribArray(1);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), usage);

    buffers_ = {vertexBuffer, indexBuffer};

    // cache uniform locations to avoid lookups in draw
    mvpLocation_ = glGetUniformLocation(shader_->getProgram(), "mvp");
    channelsLocation_ = glGetUniformLocation(shader_->getProgram(), "channels");
    alphaLocation_ = glGetUniformLocation(shader_->getProgram(), "alpha");
}

void SpriteSheetRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform.localToWorldMatrix();

    // combine the matrices into a single MVP matrix
    Matrix<4, 4> mvp = projection * (view * model);

    const auto* floatPointer = static_cast<const GLfloat*>(mvp);

    // make sure were using the shader
    shader_->bind();
    // pass the uniform data using the saved locations
    shader_->setUniformValue(glUniformMatrix4fv, mvpLocation_, 1, GL_FALSE, floatPointer);
    shader_->setUniformValue(glUniform1i, channelsLocation_, texture_->getNumberOfChannels());
    shader_->setUniformValue(glUniform1f, alphaLocation_, alpha_);

    // bind the texture
    texture_->bind();
    // bind the vertexArray
    glBindVertexArray(vao_);
    // make sure were using the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_.indexBuffer);
    // draw call
    glDrawElements(drawMode_, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}

void SpriteSheetRenderer::moveTo(const int i) {
    // R = (n - 1) / nc
    // C = (n - 1) % nc

    // x = (W + P) * C
    // Y = (H + P) * R

    // const int row = (i - 1) / numberOfColumns_;
    const int row = (numberOfRows_ - 1) - ((i - 1) / numberOfColumns_);
    const int column = (i - 1) % numberOfColumns_;

    const float leftX = static_cast<float>(column * (gridWitdh_ + padding_)) / imageWidth_;
    const float rightX = static_cast<float>((column * (gridWitdh_ + padding_)) + gridWitdh_) / imageWidth_;

    const float bottomY = static_cast<float>(row * (gridHeight_ + padding_)) / imageHeight_;
    const float topY = static_cast<float>((row * (gridHeight_ + padding_)) + gridHeight_) / imageHeight_;

    vertices_ = {
        {Vector2(static_cast<float>(-gridWitdh_) / 2, static_cast<float>(-gridHeight_) / 2), Vector2(leftX, bottomY)}, // bottom left
        {Vector2(static_cast<float>(-gridWitdh_) / 2, static_cast<float>(gridHeight_) / 2), Vector2(leftX, topY)}, // top left
        {Vector2(static_cast<float>(gridWitdh_) / 2, static_cast<float>(-gridHeight_) / 2), Vector2(rightX, bottomY)}, // bottom right
        {Vector2(static_cast<float>(gridWitdh_) / 2, static_cast<float>(gridHeight_) / 2), Vector2(rightX, topY)}, // top right
    };

    // we are working on the vao
    glBindVertexArray(vao_);
    // we are working on the vertexBuffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers_.vertexBuffer);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), usage_);
}

void SpriteSheetRenderer::setDrawMode(const int mode) {
    drawMode_ = mode;
}

SpriteSheetRenderer::~SpriteSheetRenderer() {
    // get iterator for layer in map
    auto it = allRenderers.find(layer_);

    // if the layer already exists
    if (it != allRenderers.end()) {
        // get reference to the vector of renderers for that layer
        auto& renderers = it->second;

        std::erase(renderers, this);


        // if there are no renderers, delete the vector from the map
        if (renderers.empty()) {
            allRenderers.erase(it);
        }
    }
}

void SpriteSheetRenderer::changeSpriteTexture(const int gridWitdh, const int gridHeight, const int padding, const std::shared_ptr<Texture>& texture) {
    this->texture_ = texture;
    this->gridHeight_ = gridHeight;
    this->gridWitdh_ = gridWitdh;
    this->padding_ = padding;

    this->imageWidth_ = texture_->getWidth();
    this->imageHeight_ = texture_->getHeight();

    this->numberOfColumns_ = imageWidth_ / gridWitdh_;
    this->numberOfRows_ = imageHeight_ / gridHeight_;
}
