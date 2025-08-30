#include "renderer.h"

#include <iostream>

#include "../object.h"
#include "stb_image.h"
#include "glad/gl.h"

Renderer::Renderer(Object *owner, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int usage, const std::string& texturePath, const bool flipTexture, const int textureParam, unsigned int shaderProgram, int layer) : Component(owner) {
    this->vertices = vertices;
    this->indices = indices;
    this->shaderProgram = shaderProgram;
    this->layer = layer;

    if (auto it = allRenderers.find(layer); it != allRenderers.end()) {
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

    buffers = definePrimitive(vertices, indices, usage);
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


void Renderer::Draw(const ColumnMatrix4x4& view, const ColumnMatrix4x4 &projection, const int mode) const {
    // create the model matrix from the transform
    ColumnMatrix4x4 model = ColumnMatrix4x4::identity();

    vector2 globalPosition = object->transform.getGlobalPosition();
    float globalRotation = object->transform.getGlobalRotation();
    vector2 globalScale = object->transform.getGlobalScale();
    model = model.translate(globalPosition.x, globalPosition.y, 0.0f);
    model = model.rotate_z(globalRotation);
    model = model.scale_anisotropic(globalScale.x, globalScale.y, 1.0f);

    // combine the matrices into a single MVP matrix
    ColumnMatrix4x4 mvp = projection * (view * model);

    glUseProgram(shaderProgram);

    GLint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, static_cast<const GLfloat*>(mvp));

    GLint numberOfChannelsLocation = glGetUniformLocation(shaderProgram, "channels");
    glUniform1i(numberOfChannelsLocation, numberOfChannels);

    GLint alphaLocation = glGetUniformLocation(shaderProgram, "alpha");
    glUniform1f(alphaLocation, alpha);

    drawPrimitive(buffers.indexBuffer, indices.size(), mode, vao, texture);
}

