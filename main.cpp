#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "math/mathematics.h"
#include "camera.h"
#include "object.h"
#include "components/myComponent.h"
#include "main.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "components/renderer.h"

// needed by framebuffer_size_callback() and by object.draw()
ColumnMatrix4x4 projection;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glViewport(0, 0, width, height);
    projection = projection.ortho(-static_cast<float>(width) / 2.0f, static_cast<float>(width) / 2.0f, -static_cast<float>(height) / 2.0f, static_cast<float>(height) / 2.0f, 0, 1);
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
    // define the position vertexAttribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // // enable the color vertexAttribute
    // glEnableVertexAttribArray(1);
    // define the texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, uv));
    // enable the texture attribute
    glEnableVertexAttribArray(1);

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

    mainWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn-opengl", nullptr, nullptr);
    if (!mainWindow)
    {
        std::printf("window creation failed");
        exit(0);
    }

    // lmk when something changes
    glfwSetWindowCloseCallback(mainWindow, close_callback);
    glfwSetKeyCallback(mainWindow, key_callback);
    glfwSetCursorPosCallback(mainWindow, cursor_move_callback);
    glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
    glfwSetScrollCallback(mainWindow, scroll_callback);
    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

    // were gonna use this window rn
    glfwMakeContextCurrent(mainWindow);

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
        {{212.5, 108}, {1, 1}}, // top right
        {{-212.5, 108}, {0, 1}}, // top left
        {{-212.5, -108}, {0, 0}}, // bottom left
        {{212.5, -108}, {1, 0}} // bottom right
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    // Object otherSquare({{0, 0}, 0, {1, 1}});
    // otherSquare.AddComponent<Renderer>(vertices, indices, GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/dvdvd.jpg", true, GL_CLAMP, shader);
    // otherSquare.AddComponent<myComponent>(0.5);

    Object square({{0, 0}, 0, {1, 1}});
    square.AddComponent<Renderer>(vertices, indices, GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/super-mario-transparent-background-20.png", true, GL_CLAMP, shader, 1);
    square.AddComponent<myComponent>(0.5);

    Object otherSquare({{0, 0}, 0, {1, 1}});
    otherSquare.AddComponent<Renderer>(vertices, indices, GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/dvdvd.jpg", true, GL_CLAMP, shader, 2);
    otherSquare.AddComponent<myComponent>(0.5);

    // empty the buffers to make sure its drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);

    auto mainCamera = Camera({0, 0}, 0);
    camera = &mainCamera; // doesnt matter because scope lives for entirity of app
    // update the window size initialiy
    framebuffer_size_callback(mainWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

    // variables for inside main loop to last between loops
    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;
    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!glfwWindowShouldClose(mainWindow)) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<double, std::milli> timeSinceLastUpdateInMilli = startOfLoopTime - lastLoopTime;
        double deltaTime = timeSinceLastUpdateInMilli.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        // if there are some components left to be "started", start em and remove them from the queueueue
        if (!callStartBeforeNextUpdate.empty()) {
            for (Component* component : callStartBeforeNextUpdate) {
                component->start();
            }

            callStartBeforeNextUpdate.clear();
        }

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= fixedUpdateIntervalInMilli) {
            double fixedDeltaTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            for (Object* object: allObjects) {
                object -> fixedUpdate(fixedDeltaTime);
            }

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= fixedUpdateIntervalInMilli;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        for (Object* object: allObjects) {
            object->update(deltaTime);
        }

        for (Object* object: allObjects) {
            object->lateUpdate(deltaTime);
        }

        // renders objects from back to front. the closer to 0, the closer to the camera
        std::ranges::sort(allObjects, [](const Object* a, const Object* b) {
            return a->GetComponent<Renderer>()->layer > b->GetComponent<Renderer>()->layer;
        });

        for (Object* object: allObjects) {
            if (auto renderer = object->GetComponent<Renderer>()) {
                renderer -> Draw(camera->viewMatrix, projection, GL_TRIANGLES);
            }
        }

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();

        // framerate capping
        auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        auto updateTime = std::chrono::duration<double, std::milli>(endOfLoopTime - startOfLoopTime).count();

        auto targetFrameTimeMs = 1000.0/fps;

        const auto timeToSleepMs = targetFrameTimeMs - updateTime;

        if (timeToSleepMs > 0)
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(timeToSleepMs));
    }

    // clean uo things
    glDeleteProgram(shader);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    // and exit
    exit(EXIT_SUCCESS);
}