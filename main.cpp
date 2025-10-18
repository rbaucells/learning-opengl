#include "settings.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <thread>

#include "serializationAndDeserialization/deserializer.h"
#include "scene.h"
#include "components/camera.h"
#include "components/componentExample.h"
#include "components/renderer/renderer.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "systems/input.h"
#include "systems/opengl wrappers/shader.h"
#include "systems/opengl wrappers/texture.h"
#include "systems/opengl wrappers/window.h"
#include "systems/opengl wrappers/shaderProgram.h"

// callbacks
void errorCallback(const int error, const char* description) {
    std::printf("Error with code'%d': %s\n", error, description);
}

void debugErrorCallback(GLenum source, GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userParam) {
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
};

// declarations
void drawCalls();

int main() {
    // when we get an glfwError, lmk
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        std::printf("glfw initialization failed");
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window(5, "learn-opengl");

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        printf("Failed to initialize OpenGL context\n");
        return 1;
    }

    window.reCalculateProjectionMatrix();

    // shaders
    std::shared_ptr<Shader> fragShader = Shader::create(Shader::fragment, "/Users/ricardito/Projects/learning-opengl/res/shaders/mainFragment.shader");
    std::shared_ptr<Shader> vertexShader = Shader::create(Shader::vertex, "/Users/ricardito/Projects/learning-opengl/res/shaders/mainVertex.shader");

    // shader programs
    std::shared_ptr<ShaderProgram> mainProgram = ShaderProgram::create({fragShader, vertexShader});
    // textures
    std::shared_ptr<Texture> customTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/box.jpg", GL_CLAMP, true);
    std::shared_ptr<Texture> spriteTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/super-mario-transparent-background-20.png", GL_CLAMP, true);
    std::shared_ptr<Texture> spriteSheetTexture = std::make_shared<Texture>("/Users/ricardito/Projects/learning-opengl/res/textures/f1058a91de91f29cd65527cf97cab26b861de9b5_2_1380x896.png", GL_CLAMP, true);

    Deserializer deserializer;

    Scene scene = deserializer.loadSceneFromFile("/Users/ricardito/Projects/learning-opengl/res/json files/scene.json");

    std::string jsonString = scene.serialize().toString();;

    std::cout << jsonString;

    // Scene scene;

    // auto cameraObject = scene.addObject("mainCamera", 69, {0, 0}, 0, {1, 1});
    // cameraObject->addComponent<Camera>();
    // cameraObject->addComponent<AudioListener>();
    //
    // auto square =  scene.addObject("square", 0, {0, 0}, 0, {1, 1});
    // square->addComponent<ComponentExample>();
    //
    // // renderers
    // if constexpr (constexpr int renderer = 2; renderer == 0) {
    //     std::vector<Vertex> vertices = {
    //         {{2, 2}, {1, 1}}, // top right
    //         {{-2, 2}, {0, 1}}, // top left
    //         {{-2, -2}, {0, 0}}, // bottom left
    //         {{2, -2}, {1, 0}} // bottom right
    //     };
    //
    //     std::vector<unsigned int> indices = {
    //         0, 1, 2,
    //         2, 3, 0
    //     };
    //
    //     square->addComponent<CustomRenderer>(vertices, indices, customTexture, mainProgram, 0);
    // }
    // else if constexpr (renderer == 1)
    //     square->addComponent<SpriteRenderer>(100, spriteTexture, mainProgram, 0);
    // else if constexpr (renderer == 2)
    //     square->addComponent<SpriteSheetRenderer>(69, 69, 69, 0, spriteSheetTexture, mainProgram, 0);

    // empty the buffers to make sure its drawing properly
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
    while (!Window::mainWindow->getShouldClose()) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<float> timeSinceLastUpdate = startOfLoopTime - lastLoopTime;
        float deltaTime = timeSinceLastUpdate.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

        std::string string = "Game Engine FPS: " + std::to_string(static_cast<int>(1 / deltaTime));;
        Window::mainWindow->setWindowTitle(string);

        Scene::mainScene->manageStarts();

        GLFWgamepadstate currentGamepadState;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &currentGamepadState);

        processGamepadEvents(currentGamepadState, lastGamepadState);

        accumulator += deltaTime;

        while (accumulator >= FIXED_UPDATE_INTERVAL_IN_SECONDS) {
            float fixedDeltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFixedUpdateTime).count();

            Scene::mainScene->fixedUpdate(fixedDeltaTime);

            lastFixedUpdateTime = std::chrono::high_resolution_clock::now();
            accumulator -= FIXED_UPDATE_INTERVAL_IN_SECONDS;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        Scene::mainScene->update(deltaTime);
        Scene::mainScene->lateUpdate(deltaTime);

        drawCalls();

        Scene::mainScene->manageDestructions();

        window.swapBuffers();
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
    return 0;
}

void drawCalls() {
    // why render if there is no camera
    if (!Camera::mainCamera)
        return;

    const Matrix<4, 4> cameraViewMatrix = Camera::mainCamera->getViewMatrix();
    const Matrix<4, 4> projectionViewMatrix = Window::mainWindow->getProjectionMatrix();

    // iterate through all the renderers in reverse. AKA: from back to front
    for (auto& [layer, publisher] : std::ranges::reverse_view(renderersDrawPublishers)) {
        publisher->invoke(cameraViewMatrix, projectionViewMatrix);
    }
}
