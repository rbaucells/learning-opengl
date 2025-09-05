#include "renderer.h"
#include "../object.h"
#include "stb_image.h"
#include "glad/gl.h"
#include <iostream>

Renderer::Renderer(Object *owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int usage, const std::string& texturePath, const bool flipTexture, const int textureParam, unsigned int shaderProgram, int layer) : Component(owner) {
    this->vertices = vertices;
    this->indices = indices;
    this->shaderProgram = shaderProgram;
    this->layer = layer;

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
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &numberOfChannels, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParam);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int format = 0;

    // Use a switch statement to handle all possible channel counts
    switch (numberOfChannels) {
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
            std::cout << "Unsupported number of texture channels: " << numberOfChannels << std::endl;
            stbi_image_free(data);
            return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // buffer id
    unsigned int vertexBuffer;
    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &vertexBuffer);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), usage);
    // define the position vertexAttribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, position));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, uv));
    // enable the texture attribute
    glEnableVertexAttribArray(1);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

    buffers = {vertexBuffer, indexBuffer};

    // cache uniform locations to avoid lookups in draw
    mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    channelsLocation = glGetUniformLocation(shaderProgram, "channels");
    alphaLocation = glGetUniformLocation(shaderProgram, "alpha");
}

Renderer::~Renderer() {
    // get iterator for layer in map
    auto it = allRenderers.find(layer);

    // if the layer already exists
    if (it != allRenderers.end()) {
        // get reference to the vector of renderers for that layer
        std::vector<Renderer*>& renderers = it->second;

        std::erase(renderers, this);


        // if there are no renderers, delete the vector from the map
        if (renderers.empty()) {
            allRenderers.erase(it);
        }
    }
}


void Renderer::draw(const Matrix<4, 4>& view, const Matrix<4, 4> &projection, const int mode) const {
    // create the model matrix from the transform
    const Matrix<4, 4> model = object->transform.localToWorldMatrix();

    // combine the matrices into a single MVP matrix
    Matrix<4, 4> mvp = projection * (view * model);

    const GLfloat* floatPointer = static_cast<const GLfloat*>(mvp);

    // make sure were using the shader
    glUseProgram(shaderProgram);

    // pass the uniform data using the saved locations
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, floatPointer);
    glUniform1i(channelsLocation, numberOfChannels);
    glUniform1f(alphaLocation, alpha);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // bind the vertexArray
    glBindVertexArray(vao);
    // make sure were using the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.indexBuffer);
    // draw call
    glDrawElements(mode, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
}
