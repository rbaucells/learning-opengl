#pragma once
#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    static Window* mainWindow;

    /**
     * @param width In pixels
     * @param height In pixels
     * @param title Title text
     * @param monitor Do you want to show it on a specific monitor, nullptr means default
     * @param share Used for context sharing
     */
    Window(int width, int height, const std::string& title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

    /**
     * @brief Sets mainWindow and sets opengl context
     */
    void makeCurrent();

    /**
     * @brief Useful for setting callbakcs, resizing, etc.
     * @note Automatically puts the GLFWwindow* window param
     * @tparam FUNC What type of glfw function would you like to call
     * @tparam ARGS The types of arguments passed
     * @param func The function to call
     * @param args The arguments to use, see remarks
     */
    template<typename FUNC, typename... ARGS>
    void callWindowFunction(FUNC func, ARGS ... args) {
        func(window_, args...);
    }

    bool shouldClose() const;

    ~Window();
private:
    GLFWwindow* window_;
};
