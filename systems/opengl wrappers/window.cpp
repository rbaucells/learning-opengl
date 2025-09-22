#include "glad/gl.h"
#include "window.h"

#include <iostream>

#include "../input.h"

Window* Window::mainWindow = nullptr;

// Window::Window(const int width, const int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share) {
Window::Window(const float size, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share) {
    if (mainWindow)
        throw std::runtime_error("Window already exists");
    else
        mainWindow = this;

    coordinateSystemHeight_ = size;

    coordinateSystemWidth_ = coordinateSystemHeight_ * (curWindowAspectRatio_.width / curWindowAspectRatio_.height);

    window_ = glfwCreateWindow(curWindowWidth_, curWindowHeight_, title.c_str(), monitor, share);

    glfwSetWindowUserPointer(window_, this);

    glfwSetWindowCloseCallback(window_, closeCallback);
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetCursorPosCallback(window_, cursorMoveCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetScrollCallback(window_, scrollCallback);
    glfwSetFramebufferSizeCallback(window_, updateWindowSize);

    glfwMakeContextCurrent(window_);
}

void Window::reCalculateProjectionMatrix() {
    // how many units tall
    const float screenCoordinateSystemHeight = 2 * coordinateSystemHeight_;

    // std::cout << "Screen is " << screenCoordinateSystemHeight << " units tall" << std::endl;

    // how many units wide
    const float screenCoordinateSystemWidth = screenCoordinateSystemHeight * (static_cast<float>(curWindowAspectRatio_.width) / static_cast<float>(curWindowAspectRatio_.height));

    // std::cout << "Screen is " << screenCoordinateSystemWidth << " units wide" << std::endl;

    // create projection matrix
    projection_ = Matrix<4, 4>::ortho(-screenCoordinateSystemWidth / 2.0f, screenCoordinateSystemWidth / 2.0f, -screenCoordinateSystemHeight / 2.0f, screenCoordinateSystemHeight / 2.0f, 0, 1);
}

Matrix<4, 4> Window::getProjectionMatrix() const {
    return projection_;
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window_);
}

bool Window::getShouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::setCurrentWindowSize(const int width, const int height) {
    glfwSetWindowSize(window_, width, height);
    updateWindowSize(nullptr, width, height);
}

void Window::setCurrentWindowHeight(const int height) {
    setCurrentWindowSize(getCurrentWindowWidth(), height);
}

void Window::setCurrentWindowWidth(const int width) {
    setCurrentWindowSize(width, getCurrentWindowHeight());
}

void Window::setCoordinateSystemHeight(const float height) {
    coordinateSystemHeight_ = height;
    updateWindowSize(nullptr, curWindowWidth_, curWindowHeight_);
}

Window::~Window() {
    glfwDestroyWindow(window_);
}

void Window::updateWindowSize([[maybe_unused]] GLFWwindow* glfwWindow, const int width, const int height) {
    mainWindow->curWindowWidth_ = width;
    mainWindow->curWindowHeight_ = height;
    mainWindow->curWindowAspectRatio_ = {width, height};

    glViewport(0, 0, width, height);

    mainWindow->reCalculateProjectionMatrix();
}

void Window::closeCallback(GLFWwindow* window) {
    std::printf("user closing window");
}
