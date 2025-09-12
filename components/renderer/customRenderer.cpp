#include <iostream>
#include "renderer.h"
#include "stb_image.h"
#include "../../object.h"
#include "glad/gl.h"
#include "math/vertex.h"

CustomRenderer::CustomRenderer(Object *owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int usage, const std::string& texturePath, const bool flipTexture, const int textureParam, const unsigned int shaderProgram, const int layer) : Component(owner) {
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->shaderProgram_ = shaderProgram;
    this->layer_ = layer;

    if (const auto it = allRenderers.find(layer); it != allRenderers.end()) {
        // the layer already exists
        it->second.push_back(this);
    }
    else {
        // the layer doesnt exist
        allRenderers[layer] = {this};
    }

    int width, height;
    stbi_set_flip_vertically_on_load(flipTexture);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numberOfChannels_, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParam);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int format = 0;

    // Use a switch statement to handle all possible channel counts
    switch (numberOfChannels_) {
        case 1:
            format = GL_R;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cout << "Unsupported number of texture channels: " << numberOfChannels_ << std::endl;
            stbi_image_free(data);
            return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

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
    mvpLocation_ = glGetUniformLocation(shaderProgram, "mvp");
    channelsLocation_ = glGetUniformLocation(shaderProgram, "channels");
    alphaLocation_ = glGetUniformLocation(shaderProgram, "alpha");
}

void CustomRenderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4>& projection, const int mode) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform.localToWorldMatrix();

    // combine the matrices into a single MVP matrix
    Matrix<4, 4> mvp = projection * (view * model);

    const GLfloat* floatPointer = static_cast<const GLfloat*>(mvp);

    // make sure were using the shader
    glUseProgram(shaderProgram_);

    // pass the uniform data using the saved locations
    glUniformMatrix4fv(mvpLocation_, 1, GL_FALSE, floatPointer);
    glUniform1i(channelsLocation_, numberOfChannels_);
    glUniform1f(alphaLocation_, alpha_);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture_);
    // bind the vertexArray
    glBindVertexArray(vao_);
    // make sure were using the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_.indexBuffer);
    // draw call
    glDrawElements(mode, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}

CustomRenderer::~CustomRenderer() {
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
