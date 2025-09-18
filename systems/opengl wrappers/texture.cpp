#include "texture.h"

#include <iostream>

#include "stb_image.h"
#include "glad/gl.h"

Texture::Texture(const std::string& filePath, const int textureWrap, const bool flipVertically) {
    std::printf("Constructing Texture\n");
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(filePath.c_str(), &width_, &height_, &numberOfChannels_, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture::bind() const {
    // std::printf("Binding Texture\n");
    glBindTexture(GL_TEXTURE_2D, texture_);
}

void Texture::unbind() {
    std::printf("Unbinding Texture\n");
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getNumberOfChannels() const {
    return numberOfChannels_;
}

int Texture::getWidth() const {
    return width_;
}

int Texture::getHeight() const {
    return height_;
}

Texture::~Texture() {
    std::printf("Destroying Texture\n");
    glDeleteTextures(1, &texture_);
    unbind();
}







