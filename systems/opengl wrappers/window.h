#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "../../math/matrix.h"

inline constexpr int WINDOW_WIDTH = 1280;
inline constexpr int WINDOW_HEIGHT = 720;

class Window {
public:
    static Window* mainWindow;

    Window(float size, const std::string& title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

    void reCalculateProjectionMatrix();
    Matrix<4,4> getProjectionMatrix() const;

    void swapBuffers() const;

    [[nodiscard]] bool getShouldClose() const;

    // getters
    int getCurrentWindowWidth() const {
        return curWindowWidth_;
    }

    int getCurrentWindowHeight() const {
        return curWindowHeight_;
    }

    float getCoordinateSystemHeight() const {
        return coordinateSystemHeight_;
    }

    float getCoordinateSystemWidth() const {
        return coordinateSystemWidth_;
    }

    // setters
    void setCurrentWindowSize(int width, int height);

    void setCurrentWindowHeight(int height);
    void setCurrentWindowWidth(int width);

    void setCoordinateSystemHeight(float height);

    ~Window();

private:
    struct AspectRatio {
        int width, height;
    };

    GLFWwindow* window_;

    // callbacks
    static void updateWindowSize(GLFWwindow* glfwWindow, int width, int height);
    static void closeCallback(GLFWwindow* window);

    Matrix<4,4> projection_;

    // set default window size
    int curWindowWidth_ = WINDOW_WIDTH;
    int curWindowHeight_ = WINDOW_HEIGHT;
    AspectRatio curWindowAspectRatio_ = {WINDOW_WIDTH, WINDOW_HEIGHT};

    // can be set by user
    float coordinateSystemHeight_;

    // calculated from coordinateSystemHeight_. Cannot be set by user
    float coordinateSystemWidth_;
};
