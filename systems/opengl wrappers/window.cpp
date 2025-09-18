#include "window.h"

Window::Window(const int width, const int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share) {
    window_ = glfwCreateWindow(width, height, title.c_str(), monitor, share);
}

Window* Window::mainWindow = nullptr;

void Window::makeCurrent() {
    glfwMakeContextCurrent(window_);
    mainWindow = this;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

Window::~Window() {
    glfwDestroyWindow(window_);
}
