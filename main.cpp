#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "main.h"
#include "object.h"
#include "components/camera.h"
#include "components/rotateComponent.h"
#include "math/mathematics.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>

#include "list.h"
#include "components/renderer.h"
#include "systems/input.h"
#include "systems/workQueue.h"

// needed by framebuffer_size_callback() and by object.draw()
Matrix<4,4> projection;

// callbacks
void errorCallback(const int error, const char *description) {
    std::printf("Error with code'%d': %s\n", error, description);
}

void debugErrorCallback(GLenum source, GLenum type, GLuint id, const GLenum severity, const GLsizei length, const GLchar *message, const void *userParam) {
    std::string messageString(message, length);
    std::cout << severity << ": OpenGL error: %s\n" << messageString.c_str() << std::endl;
};

void closeCallback(GLFWwindow *window) {
    std::printf("user closing window");
}

void framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    projection = Matrix<4, 4>::ortho(-static_cast<float>(width) / 2.0f, static_cast<float>(width) / 2.0f, -static_cast<float>(height) / 2.0f, static_cast<float>(height) / 2.0f, 0, 1);
}

// shading
std::string getShaderString(const std::string &filePath) {
    std::fstream fileStream(filePath);

    std::string line;
    std::stringstream ss;

    while (getline(fileStream, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

unsigned int compileShader(unsigned int type, const std::string &source) {
    const unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char message[lenght];
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "Failed to compile shader! \n" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    // we can delete the shaders because they are already attached to our program
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}

void handleQueue(const double deltaTime) {
    for (const auto& [action, owner] : nextFrameQueue) {
        action();
    }

    nextFrameQueue.clear();

    for (auto it = timedQueue.begin(); it != timedQueue.end(); ) {
        it->time -= static_cast<float>(deltaTime);

        if (it->time <= 0) {
            it->action();
            it = timedQueue.erase(it);
            continue;
        }

        ++it;
    }

    for (auto it = conditionalQueue.begin(); it != conditionalQueue.end(); ) {
        if (it->condition()) {
            it->action();
            it = conditionalQueue.erase(it);
            continue;
        }

        ++it;
    }
}

int main() {
    // when we get an glfwError, lmk
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        std::printf("glfw initialization failed");
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainWindow = glfwCreateWindow(screenWidth, screenHeight, "learn-opengl", nullptr, nullptr);
    if (!mainWindow) {
        std::printf("window creation failed");
        exit(0);
    }
    // lmk when something changes
    glfwSetWindowCloseCallback(mainWindow, closeCallback);
    glfwSetKeyCallback(mainWindow, key_callback);
    glfwSetCursorPosCallback(mainWindow, cursor_move_callback);
    glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
    glfwSetScrollCallback(mainWindow, scroll_callback);
    glfwSetFramebufferSizeCallback(mainWindow, framebufferSizeCallback);

    // were gonna use this window rn
    glfwMakeContextCurrent(mainWindow);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        printf("Failed to initialize OpenGL context\n");
        exit(0);
    }

    glViewport(0, 0, screenWidth, screenHeight);

    // get the strings from the shader files
    auto const vertexShader = getShaderString("/Users/ricardito/CLionProjects/OpenGL/res/shaders/vertex.shader");
    auto const fragmentShader = getShaderString("/Users/ricardito/CLionProjects/OpenGL/res/shaders/fragment.shader");
    // compile the shaders
    unsigned int shader = createShader(vertexShader, fragmentShader);
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

    Object camera("mainCamera", 69, {0, 0}, 0, {1,1});
    auto cameraComponent = camera.addComponent<Camera>();
    if (auto cam = cameraComponent.lock()) {
        cam->setMain();
    }

    Object origin1("origin1", 0, {0, 0}, 0, {1, 1});
    // origin1.addComponent<RotateComponent>(-45);

    Object origin2("origin2", 0, {0, 0}, 0, {1, 1});
    // origin2.addComponent<RotateComponent>(45);

    Object square("square", 0, {200, 0}, 0, {1, 1}, &origin1.transform);
    square.addComponent<Renderer>(vertices, indices, GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/super-mario-transparent-background-20.png", true, GL_CLAMP, shader, 2);
    square.addComponent<RotateComponent>(45);

    Object otherSquare("other square", 0, {600, 0}, 0, {1, 1}, &origin2.transform);
    otherSquare.addComponent<Renderer>(vertices, indices, GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/dvdvd.jpg", true, GL_CLAMP, shader, 1);

    // empty the buffers to make sure its drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);


    // update the window size initialiy
    framebufferSizeCallback(mainWindow, screenWidth, screenHeight);

    // variables for inside main loop to last between loops
    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;
    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!glfwWindowShouldClose(mainWindow)) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<double> timeSinceLastUpdate = startOfLoopTime - lastLoopTime;
        double deltaTime = timeSinceLastUpdate.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        // if there are some components left to be "started", start em and remove them from the queueueue
        if (!componentsToInitialize.empty()) {
            for (auto it = componentsToInitialize.begin(); it != componentsToInitialize.end(); ) {
                if (auto comp = it->lock()) {
                    comp->awake();
                    ++it;
                }
                else {
                    it = componentsToInitialize.erase(it);
                }
            }

            // if the object is supposed to be active, call the onEnable
            for (auto component: componentsToInitialize) {
                if (auto comp = component.lock()) {
                    if (comp->object->getActive()) {
                        comp->onEnable();
                    }
                }
            }

            // start
            for (auto component: componentsToInitialize) {
                if (auto comp = component.lock()) {
                    comp->start();
                }
            }

            componentsToInitialize.clear();
        }

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= FIXED_UPDATE_INTERVAL_IN_SECONDS) {
            double fixedDeltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            fixedUpdateEvent.invoke(fixedDeltaTime);

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= FIXED_UPDATE_INTERVAL_IN_SECONDS;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        updateEvent.invoke(deltaTime);
        lateUpdateEvent.invoke(deltaTime);

        Matrix<4, 4> cameraViewMatrix = Camera::mainCamera->getViewMatrix();
        // iterate through all the renderers in reverse. AKA: from back to front
        for (auto &renderersInLayer: std::ranges::reverse_view(allRenderers)) {
            for (const auto &renderer: renderersInLayer.second) {
                renderer->draw(cameraViewMatrix, projection, GL_TRIANGLES);
            }
        }

        handleQueue(deltaTime);

        for (Object* object : waitingLineOfDeath) {
            object->destroyImmediately();
            // delete object;
        }

        waitingLineOfDeath.clear();

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();

        // framerate capping
        auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        auto updateTime = std::chrono::duration<double, std::milli>(endOfLoopTime - startOfLoopTime).count();

        auto targetFrameTimeMs = 1000.0 / FPS;

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