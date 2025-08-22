#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "main.h"
#include "components/collider.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "linmath/linmath.h"

mat4x4 projection;

// callbacks
void error_callback(int error, const char* description) {
    std::printf("Error with code'%d': %s\n", error, description);
}
void debugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    std::string messageString(message, length);
    std::cout << severity << ": OpenGL error: %s\n" << messageString.c_str() << std::endl;
};
void close_callback(GLFWwindow* window) {
    std::printf("user closing window");
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glViewport(0, 0, width, height);
    mat4x4_ortho(projection, -width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, 1, -1000);
}
// shading
std::string GetShaderString(const std::string& filePath) {
    std::fstream fileStream(filePath);

    std::string line;
    std::stringstream ss;

    while (getline(fileStream, line)) {
        ss << line << '\n';
    }

    return ss.str();
}
unsigned int CompileShader(unsigned int type, const std::string& source) {
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char message[lenght];
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "Failed to compile shader! \n" <<  message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    // we can delete the shaders because they are already attached to our program
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}

// primitive
Buffers definePrimitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int usage) {
    // buffer id
    unsigned int vertexBuffer;
    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &vertexBuffer);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), usage);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // enable the color vertexAttribute
    glEnableVertexAttribArray(1);
    // define the texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, uv));
    // enable the texture attribute
    glEnableVertexAttribArray(2);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

    return {vertexBuffer, indexBuffer};
}
void drawPrimitive(unsigned int indexBuffer, const int indicesCount, unsigned int mode, unsigned int vao, unsigned int texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    // bind the vertexArray
    glBindVertexArray(vao);
    // make sure were using the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // draw call
    glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, 0);
}

int main() {
    // when we get an error, lmk
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::printf("glfw initialization failed");
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn-opengl", nullptr, nullptr);
    if (!window)
    {
        std::printf("window creation failed");
        exit(0);
    }

    // lmk when something changes
    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // were gonna use this window rn
    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        printf("Failed to initialize OpenGL context\n");
        exit(0);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // get the strings from the shader files
    auto const vertexShader = GetShaderString("/Users/ricardito/CLionProjects/OpenGL/res/shaders/vertex.shader");
    auto const fragmentShader = GetShaderString("/Users/ricardito/CLionProjects/OpenGL/res/shaders/fragment.shader");
    // compile the shaders
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    // yes im gonna use them
    glUseProgram(shader);

    std::vector<Vertex> vertices = {
        {{128, 128}, {1, 0, 0, 1}, {1, 1}},
        {{128, -128}, {0, 1, 0, 1}, {1, 0}},
        {{-128, -128}, {0, 0, 1, 1}, {0, 0}},
        {{-128, 128}, {1, 1, 0, 1}, {0, 1}}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Object square(vertices, indices, shader, {{0, 0}, 0, {1, 1}});
    square.Define(GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/texture.jpg");
    square.AddComponent<Collider>();
    // empty the buffers to make sure its drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);

    Camera camera({0, 0}, 0);

    framebuffer_size_callback(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    // main update loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (Object* object: allObjects) {
            object->Update();
        }

        for (Object* object: allObjects) {
            object->LateUpdate();
        }

        for (Object* object: allObjects) {
            object->Draw(GL_TRIANGLES, camera.viewMatrix, projection);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean uo things
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    // and exit
    exit(EXIT_SUCCESS);
}