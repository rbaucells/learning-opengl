#include <iostream>
#include "renderer.h"
#include "../../object.h"
#include "glad/gl.h"
#include "../../math/vertex.h"
#include "../../systems/opengl wrappers/shader.h"
#include "../../systems/opengl wrappers/texture.h"

SpriteRenderer::SpriteRenderer(Object* owner, Vector2 size, unsigned int usage, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer) : RendererBase(owner) {
    vertices_ = {
        {{-size.x / 2, -size.y / 2}, {0, 0}}, // bottom left
        {{-size.x / 2, size.y / 2}, {0, 1}}, // top left
        {{size.x / 2, -size.y / 2}, {1, 0}}, // bottom right
        {{size.x / 2, size.y / 2}, {1, 1}}, // top right
    };

    indices_ = {
        0, 1, 2,
        1, 3, 2
    };

    this->shader_ = shader;
    this->layer_ = layer;
    this->texture_ = texture;

    if (const auto it = allRenderers.find(layer); it != allRenderers.end()) {
        // the layer already exists
        it->second.push_back(this);
    }
    else {
        // the layer doesnt exist
        allRenderers[layer] = {this};
    }

    texture_->bind();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // buffer id
    unsigned int vertexBuffer;
    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &vertexBuffer);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), usage);
    // define the position vertexAttribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, uv));
    // enable the texture attribute
    glEnableVertexAttribArray(1);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), usage);

    buffers_ = {vertexBuffer, indexBuffer};

    // cache uniform locations to avoid lookups in draw
    // mvpLocation_ = glGetUniformLocation(shader_->getProgram(), "mvp");
    // channelsLocation_ = glGetUniformLocation(shader_->getProgram(), "channels");
    // alphaLocation_ = glGetUniformLocation(shader_->getProgram(), "alpha");

    mvpLocation_ = shader->getUniformLocation("mvp");
    channelsLocation_ = shader->getUniformLocation("channels");
    alphaLocation_ = shader->getUniformLocation("alpha");
}

void SpriteRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform.localToWorldMatrix();

    // combine the matrices into a single MVP matrix
    Matrix<4, 4> mvp = projection * (view * model);

    const auto* floatPointer = static_cast<const GLfloat*>(mvp);

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

void SpriteRenderer::setDrawMode(const int mode) {
    drawMode_ = mode;
}

SpriteRenderer::~SpriteRenderer() {
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

void SpriteRenderer::changeSpriteTexture(const std::shared_ptr<Texture>& texture) {
    this->texture_ = texture;
}
