#pragma once
#include <string>

/**
 * @warning Textures MUST outlive the lifetime of the object that uses them, they should be created either in a greater scope than the renderer/component that uses them or as a shared-ptr
 * @warning The destructor will delete the texture from the GPU
*/
class Texture {
public:
    Texture(const std::string& filePath, int textureParam, bool flipVertically = true);

    void bind() const;
    static void unbind();

    unsigned char* getData();
    unsigned char* getData() const;

    int getNumberOfChannels() const;

    int getWidth() const;
    int getHeight() const;

    ~Texture();

private:
    unsigned int texture_ = -1;
    unsigned char* data_ = nullptr;

    int width_ = 0;
    int height_ = 0;

    int numberOfChannels_ = 0;
};
