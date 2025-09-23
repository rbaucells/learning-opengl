// #include <iostream>
// #include "renderer.h"
// #include "../../object.h"
// #include "glad/gl.h"
// #include "../../math/vertex.h"
// #include "../../systems/opengl wrappers/shader.h"
// #include "../../systems/opengl wrappers/texture.h"
//
// SpriteSheetRenderer::SpriteSheetRenderer(Object* owner, Vector2 size, int gridWidth, int gridHeight, int padding, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, int layer, Usage usage, DrawMode drawMode) : RendererBase(owner) {
//     this->texture_ = texture;
//     this->gridWitdh_ = gridWidth;
//     this->gridHeight_ = gridHeight;
//     this->imageHeight_ = texture_->getHeight();
//     this->imageWidth_ = texture_->getWidth();
//     this->padding_ = padding;
//
//     this->size_ = size;
//
//     this->shader_ = shader;
//
//     this->usage_ = usage;
//     this->drawMode = drawMode;
//
//     this->layer_ = layer;
//
//     // simple division while rounding down to ensure no cropped out things
//     this->numberOfColumns_ = imageWidth_ / gridWitdh_;
//     this->numberOfRows_ = imageHeight_ / gridHeight_;
//
//     constexpr int leftX = 0;
//     constexpr int bottomY = 0;
//
//     const float topY = static_cast<float>(gridHeight_) / static_cast<float>(imageHeight_);
//     const float rightX = static_cast<float>(gridWitdh_) / static_cast<float>(imageWidth_);
//
//     this->vertices_ = {
//         {{-size_.x / 2, -size_.y / 2}, {leftX, bottomY}}, // bottom left
//         {{-size_.x / 2, size_.y / 2}, {leftX, topY}}, // top left
//         {{size_.x / 2, -size_.y / 2}, {rightX, bottomY}}, // bottom right
//         {{size_.x / 2, size_.y / 2}, {rightX, topY}}, // top right
//     };
//
//     this->indices_ = {
//         0, 1, 2,
//         1, 3, 2
//     };
//
//     texture_->bind();
//
//     glGenVertexArrays(1, &vertexArrayObject_);
//     glBindVertexArray(vertexArrayObject_);
//
//     // generate 1 buffer and assign the id into uint buffer ^
//     glGenBuffers(1, &vertexBuffer_);
//     // I am going to work with this buffer. select it
//     glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
//
//     // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
//     glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices_.size() * sizeof(Vertex)), vertices_.data(), usage);
//     // define the position vertexAttribute
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
//     // enable the position vertexAttribute
//     glEnableVertexAttribArray(0);
//     // define the texture attribute
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));
//     // enable the texture attribute
//     glEnableVertexAttribArray(1);
//
//     glGenBuffers(1, &indexBuffer_);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices_.size() * sizeof(unsigned int)), indices_.data(), usage);
//
//     // cache uniform locations to avoid lookups in draw
//     mvpLocation_ = glGetUniformLocation(shader_->getProgram(), "mvp");
//     channelsLocation_ = glGetUniformLocation(shader_->getProgram(), "channels");
//     alphaLocation_ = glGetUniformLocation(shader_->getProgram(), "alpha");
//
//     addToAllRenderers(layer_);
// }
//
// void SpriteSheetRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection) const {
//     // create the model matrix from the transform
//     const Matrix<4, 4> model = object->transform.localToWorldMatrix();
//
//     // combine the matrices into a single MVP matrix
//     Matrix<4, 4> mvp = projection * (view * model);
//
//     const auto* floatPointer = static_cast<const GLfloat*>(mvp);
//
//     // make sure were using the shader
//     shader_->bind();
//     // pass the uniform data using the saved locations
//     shader_->setUniformValue(glUniformMatrix4fv, mvpLocation_, 1, GL_FALSE, floatPointer);
//     shader_->setUniformValue(glUniform1i, channelsLocation_, texture_->getNumberOfChannels());
//     shader_->setUniformValue(glUniform1f, alphaLocation_, alpha_);
//
//     // bind the texture
//     texture_->bind();
//     // bind the vertexArray
//     glBindVertexArray(vertexArrayObject_);
//     // make sure were using the index buffer
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
//     // draw call
//     glDrawElements(drawMode, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
// }
//
// Bounds SpriteSheetRenderer::getBounds() const {
//     Bounds bounds = {};
//
//     Vertex maxX;
//     Vertex minX;
//
//     Vertex maxY;
//     Vertex minY;
//
//     for (const auto& [position, uv] : vertices_) {
//         if (position.x > maxX.position.x) {
//             maxX.position.x = position.x;
//         }
//         else if (position.x < minX.position.x) {
//             minX.position.x = position.x;
//         }
//
//         if (position.y > maxY.position.y) {
//             maxY.position.y = position.y;
//         }
//         else if (position.y < minY.position.y) {
//             minY.position.y = position.y;
//         }
//     }
//
//     bounds.center = object->transform.getGlobalPosition();
//
//     bounds.max = {maxX.position.x / 2.f, maxY.position.y / 2.f};
//     bounds.min = {minX.position.x / 2.f, minY.position.y / 2.f};
//
//     bounds.size = bounds.max - bounds.min;
//
//     return bounds;
// }
//
// SpriteSheetRenderer::~SpriteSheetRenderer() {
//     removeFromAllRenderers();
// }
//
// void SpriteSheetRenderer::removeFromAllRenderers() const {
//     // get iterator for layer in map
//     auto it = allRenderers.find(layer_);
//
//     // if the layer already exists
//     if (it != allRenderers.end()) {
//         // get reference to the vector of renderers for that layer
//         auto& renderers = it->second;
//
//         std::erase(renderers, this);
//
//         // if there are no renderers, delete the vector from the map
//         if (renderers.empty()) {
//             allRenderers.erase(it);
//         }
//     }
// }
//
// void SpriteSheetRenderer::addToAllRenderers(const int layer) {
//     // add the renderer to allRenderers
//     if (const auto it = allRenderers.find(layer); it != allRenderers.end()) {
//         // the layer already exists
//         it->second.push_back(this);
//     }
//     else {
//         // the layer doesnt exist
//         allRenderers[layer] = {this};
//     }
// }
