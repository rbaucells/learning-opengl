#include "application.h"

#include <iostream>
#include <ranges>
#include <thread>

#include "scene.h"
#include "settings.h"
#include "components/camera.h"
#include "components/renderer/renderer.h"
#include "systems/input.h"

Application::Application(const std::string& scenePath) {
    mainScene = Scene::deserialize(scenePath);
    // when we get an glfwError, lmk
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        throw ApplicationError("GLFW initialization failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainWindow = std::make_unique<Window>(5, "learn-opengl");

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        throw ApplicationError("Failed to initialize OpenGL context");
    }
}

void Application::run() {
    // empty things to make sure drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);

    // variables for inside main loop to last between loops
    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.0;
    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();

    // ReSharper disable once CppTooWideScope
    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!mainWindow->getShouldClose()) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<float> timeSinceLastUpdate = startOfLoopTime - lastLoopTime;
        const float deltaTime = timeSinceLastUpdate.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        std::string string = "Game Engine FPS: " + std::to_string(static_cast<int>(1 / deltaTime));;
        mainWindow->setWindowTitle(string);

        mainScene->manageStarts();

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= Settings::FIXED_UPDATE_INTERVAL_IN_SECONDS) {
            const float fixedDeltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            mainScene->fixedUpdate(fixedDeltaTime);

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= Settings::FIXED_UPDATE_INTERVAL_IN_SECONDS;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        mainScene->update(deltaTime);
        mainScene->lateUpdate(deltaTime);

        // why render if there is no camera
        if (Camera::mainCamera) {
            const Matrix<4, 4> cameraViewMatrix = Camera::mainCamera->getViewMatrix();
            const Matrix<4, 4> projectionViewMatrix = mainWindow->getProjectionMatrix();

            // iterate through all the renderers in reverse. AKA: from back to front
            for (auto& [layer, publisher] : std::ranges::reverse_view(renderersDrawPublishers)) {
                publisher->invoke(cameraViewMatrix, projectionViewMatrix);
            }
        }

        mainScene->manageDestructions();

        mainWindow->swapBuffers();
        glfwPollEvents();

        // framerate capping
        const auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        const float updateTime = std::chrono::duration<float, std::milli>(endOfLoopTime - startOfLoopTime).count();

        const float targetFrameTimeMs = 1000.0f / Settings::fps;

        if (const auto timeToSleepMs = targetFrameTimeMs - updateTime; timeToSleepMs > 0)
            std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(timeToSleepMs));
    }
}

Application::~Application() {
    glfwTerminate();
}

std::unique_ptr<Window> Application::mainWindow = nullptr;
std::unique_ptr<Scene> Application::mainScene = nullptr;

void Application::errorCallback(const int error, const char* description) {
    std::printf("Error with code'%d': %s\n", error, description);
}

void Application::debugErrorCallback(GLenum source, GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "HIGH: Debug message (" << id << "): " << message << std::endl;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "MEDIUM: Debug message (" << id << "): " << message << std::endl;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "LOW: Debug message (" << id << "): " << message << std::endl;
            break;
        default:
            std::cerr << "Debug message (" << id << "): " << message << std::endl;
            break;
    }
}
