#include "application.h"

#include <iostream>
#include <ranges>
#include <thread>

#include "scene.h"
#include "settings.h"
#include "components/camera.h"
#include "components/renderer/renderer.h"
#include "serialization/deserializer.h"
#include "systems/input.h"

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

Application::Application(const std::string& scenePath) {
    Deserializer deserializer;
    Scene scene = deserializer.loadSceneFromFile(scenePath);
    scene.setMain();

    // when we get an glfwError, lmk
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        throw ApplicationError("GLFW initialization failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = std::make_unique<Window>(5, "learn-opengl");

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        throw ApplicationError("Failed to initialize OpenGL context");
    }
}

void Application::run() {
    // variables for inside main loop to last between loops
    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.0;
    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();

    // ReSharper disable once CppTooWideScope
    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!Window::mainWindow->getShouldClose()) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<float> timeSinceLastUpdate = startOfLoopTime - lastLoopTime;
        const float deltaTime = timeSinceLastUpdate.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        std::string string = "Game Engine FPS: " + std::to_string(static_cast<int>(1 / deltaTime));;
        Window::mainWindow->setWindowTitle(string);

        Scene::mainScene->manageStarts();

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= Settings::FIXED_UPDATE_INTERVAL_IN_SECONDS) {
            const float fixedDeltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            Scene::mainScene->fixedUpdate(fixedDeltaTime);

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= Settings::FIXED_UPDATE_INTERVAL_IN_SECONDS;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        Scene::mainScene->update(deltaTime);
        Scene::mainScene->lateUpdate(deltaTime);

        // why render if there is no camera
        if (!Camera::mainCamera)
            return;

        const Matrix<4, 4> cameraViewMatrix = Camera::mainCamera->getViewMatrix();
        const Matrix<4, 4> projectionViewMatrix = Window::mainWindow->getProjectionMatrix();

        // iterate through all the renderers in reverse. AKA: from back to front
        for (auto& [layer, publisher] : std::ranges::reverse_view(renderersDrawPublishers)) {
            publisher->invoke(cameraViewMatrix, projectionViewMatrix);
        }

        Scene::mainScene->manageDestructions();

        window->swapBuffers();
        glfwPollEvents();

        // framerate capping
        auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        float updateTime = std::chrono::duration<float, std::milli>(endOfLoopTime - startOfLoopTime).count();

        float targetFrameTimeMs = 1000.0f / Settings::fps;

        if (const auto timeToSleepMs = targetFrameTimeMs - updateTime; timeToSleepMs > 0)
            std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(timeToSleepMs));
    }
}

Application::~Application() {
    glfwTerminate();
}
