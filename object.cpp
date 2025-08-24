#include "object.h"
#include <vector>
#include "stb_image.h"
#include "components/component.h"
#include "glad/gl.h"

Object::Object(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shaders, Transform transform) {
    this->shaders = shaders;
    this->vertices = vertices;
    this->indices = indices;
    this->transform = transform;

    glGenVertexArrays(1, &vertexArrayObject);
    glUseProgram(shaders); // activate shaders and get uniform locations

    allObjects.push_back(this);
}

Object::~Object() {
    std::erase(allObjects, this);
}

void Object::Define(const unsigned int usage, const std::string &textureFilePath) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindVertexArray(vertexArrayObject);
    buffers = definePrimitive(vertices, indices, usage);
}

void Object::Draw(const unsigned int mode, ColumnMatrix4x4 view, ColumnMatrix4x4 projection) const {
    // create the model matrix from the transform
    ColumnMatrix4x4 model = ColumnMatrix4x4::identity();
    model = model.translate(transform.position.x, transform.position.y, 0.0f);
    model = model.rotate_z(transform.rotation);
    model = model.scale_anisotropic(transform.scale.x, transform.scale.y, 1.0f);

    // combine the matrices into a single MVP matrix
    ColumnMatrix4x4 mvp;
    mvp = projection * (view * model);

    GLint mvpLocation = glGetUniformLocation(shaders, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*) mvp);

    glUseProgram(shaders);
    drawPrimitive(buffers.indexBuffer, indices.size(), mode, vertexArrayObject, texture);
}

void Object::update(const double deltaTime) const {
    for (const auto& component: components) {
        component -> update(deltaTime);
    }
}
void Object::fixedUpdate(const double fixedDeltaTime) const {
    for (const auto& component: components) {
        component -> fixedUpdate(fixedDeltaTime);
    }
}
void Object::lateUpdate(const double deltaTime) const {
    for (const auto& component: components) {
        component -> lateUpdate(deltaTime);
    }
}