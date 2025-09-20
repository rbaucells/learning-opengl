#include "main.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <thread>

#include "list.h"
#include "object.h"
#include "components/camera.h"
#include "components/rotateComponent.h"
#include "components/renderer/renderer.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "math/vertex.h"
#include "systems/input.h"
#include "systems/opengl wrappers/shader.h"
#include "systems/opengl wrappers/texture.h"
#include "systems/opengl wrappers/window.h"
#include "systems/audio/audioListener.h"
#include "systems/audio/audioSource.h"

// needed by framebuffer_size_callback() and by object.draw()
Matrix<4, 4> projection;

// callbacks
void errorCallback(const int error, const char* description) {
    std::printf("Error with code'%d': %s\n", error, description);
}

void debugErrorCallback(GLenum source, GLenum type, GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParam) {
    const std::string messageString(message, length);
    std::cout << severity << ": OpenGL error: %s\n" << messageString.c_str() << std::endl;
};

void closeCallback(GLFWwindow* window) {
    std::printf("user closing window");
}

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    projection = Matrix<4, 4>::ortho(-static_cast<float>(width) / 2.0f, static_cast<float>(width) / 2.0f, -static_cast<float>(height) / 2.0f, static_cast<float>(height) / 2.0f, 0, 1);
}

// declarations
void initializeObjects();
void destroyObjects();
void drawCalls();

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

    Window window(screenWidth, screenHeight, "learn-opengl");

    window.callWindowFunction(glfwSetWindowCloseCallback, closeCallback);
    window.callWindowFunction(glfwSetKeyCallback, key_callback);
    window.callWindowFunction(glfwSetCursorPosCallback, cursor_move_callback);
    window.callWindowFunction(glfwSetMouseButtonCallback, mouse_button_callback);
    window.callWindowFunction(glfwSetScrollCallback, scroll_callback);
    window.callWindowFunction(glfwSetFramebufferSizeCallback, framebufferSizeCallback);

    window.makeCurrent();

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        printf("Failed to initialize OpenGL context\n");
        exit(0);
    }

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

    Object camera("mainCamera", 69, {0, 0}, 0, {1, 1});
    auto cameraComponent = camera.addComponent<Camera>();
    auto listenerComponent = camera.addComponent<AudioListener>();

    if (auto cam = cameraComponent.lock()) {
        cam->setMain();
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("/Users/ricardito/Projects/learning-opengl/res/shaders/vertex.shader", "/Users/ricardito/Projects/learning-opengl/res/shaders/fragment.shader");

    std::shared_ptr<Texture> mainTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/super-mario-transparent-background-20.png", GL_CLAMP, true);
    std::shared_ptr<Texture> spriteSheetTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/f1058a91de91f29cd65527cf97cab26b861de9b5_2_1380x896.png", GL_CLAMP, true);
    // std::shared_ptr<Texture> spriteSheetTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/fonts/squiggly-wiggly-white.png", GL_CLAMP, true);
    Object origin1("origin1", 0, {0, 0}, 0, {1, 1});

    // Object origin2("origin2", 0, {0, 0}, 0, {1, 1});

    Object square("square", 0, {0, 0}, 0, {15, 15}, &origin1.transform);
    // square.addComponent<SpriteSheetRenderer>(69, 69, 0, GL_STATIC_DRAW, spriteSheetTexture, shader, 2);
    square.addComponent<SpriteRenderer>(Vector2(320, 426), GL_STATIC_DRAW, mainTexture, shader, 2);
    // square.addComponent<CustomRenderer>(vertices, indices, GL_STATIC_DRAW, mainTexture, shader, 2);
    square.addComponent<RotateComponent>(45);
    square.addComponent<AudioSource>("/Users/ricardito/Projects/learning-opengl/res/audios/file_example_WAV_1MG.wav");

    // empty the buffers to make sure its drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);

    // update the window size initialiy
    window.callWindowFunction(framebufferSizeCallback, screenWidth, screenHeight);

    // variables for inside main loop to last between loops
    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.0;
    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();

    // ReSharper disable once CppTooWideScope
    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!Window::mainWindow->shouldClose()) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<float> timeSinceLastUpdate = startOfLoopTime - lastLoopTime;
        float deltaTime = timeSinceLastUpdate.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        initializeObjects();

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= FIXED_UPDATE_INTERVAL_IN_SECONDS) {
            float fixedDeltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            fixedUpdateEvent.invoke(fixedDeltaTime);

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= FIXED_UPDATE_INTERVAL_IN_SECONDS;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        updateEvent.invoke(deltaTime);
        lateUpdateEvent.invoke(deltaTime);

        drawCalls();

        destroyObjects();

        Window::mainWindow->callWindowFunction(glfwSwapBuffers);
        glfwPollEvents();

        // framerate capping
        auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        float updateTime = std::chrono::duration<float, std::milli>(endOfLoopTime - startOfLoopTime).count();

        float targetFrameTimeMs = 1000.0f / FPS;

        if (const auto timeToSleepMs = targetFrameTimeMs - updateTime; timeToSleepMs > 0)
            std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(timeToSleepMs));
    }

    // clean up things
    glfwTerminate();
    // and exit
    exit(EXIT_SUCCESS);
}

void initializeObjects() {
    // if there are some components left to be "started", start em and remove them from the queueueue
    if (!componentsToInitialize.empty()) {
        for (auto it = componentsToInitialize.begin(); it != componentsToInitialize.end();) {
            if (const auto comp = it->lock()) {
                comp->awake();
                ++it;
            }
            else {
                it = componentsToInitialize.erase(it);
            }
        }

        // if the object is supposed to be active, call the onEnable
        for (const auto& component : componentsToInitialize) {
            if (auto comp = component.lock()) {
                if (comp->object->getActive()) {
                    comp->onEnable();
                }
            }
        }

        // start
        for (const auto& component : componentsToInitialize) {
            if (auto comp = component.lock()) {
                comp->start();
            }
        }

        componentsToInitialize.clear();
    }
}

void destroyObjects() {
    for (Object* object : waitingLineOfDeath) {
        object->destroyImmediately();
        // delete object; <- put this when objects moved to heap
    }

    waitingLineOfDeath.clear();
}

void drawCalls() {
    // iterate through all the renderers in reverse. AKA: from back to front
    const Matrix<4, 4> cameraViewMatrix = Camera::mainCamera->getViewMatrix();
    for (auto& renderersInLayer : std::ranges::reverse_view(allRenderers)) {
        for (const auto& renderer : renderersInLayer.second) {
            renderer->draw(cameraViewMatrix, projection);
        }
    }
}
