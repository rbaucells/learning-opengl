#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "camera.h"
#include "object.h"
#include "components/Moving.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "main.h"

#include <thread>

#include "input.h"
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

void invokeInput(const InputContext& context) {
    for (const Object* object : allObjects) {
        for (const auto& component : object->components) {
            for (const InputAction compAction : component->actionsToListenToo) {
                if (compAction == context.action) {
                    component->onInput(context);
                }
            }
        }
    }
}

void cursor_move_callback(GLFWwindow* window, double xpos, double ypos) {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);
    invokeInput(InputContext(mouse_moved, {x, y}));
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    bool pressed = action == GLFW_PRESS;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            invokeInput(InputContext(left_click, pressed));
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            invokeInput(InputContext(right_click, pressed));
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            invokeInput(InputContext(middle_click, pressed));
            break;
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    const auto x = static_cast<float>(xoffset);
    const auto y = static_cast<float>(yoffset);
    invokeInput(InputContext(scroll_x, x));
    invokeInput(InputContext(scroll_y, y));
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) {
        return;
    }
    const bool pressed = (action == GLFW_PRESS);

    switch (key) {
        case GLFW_KEY_ESCAPE:
            invokeInput(InputContext(esc, pressed));
            break;
        case GLFW_KEY_F1:
            invokeInput(InputContext(f1, pressed));
            break;
        case GLFW_KEY_F2:
            invokeInput(InputContext(f2, pressed));
            break;
        case GLFW_KEY_F3:
            invokeInput(InputContext(f3, pressed));
            break;
        case GLFW_KEY_F4:
            invokeInput(InputContext(f4, pressed));
            break;
        case GLFW_KEY_F5:
            invokeInput(InputContext(f5, pressed));
            break;
        case GLFW_KEY_F6:
            invokeInput(InputContext(f6, pressed));
            break;
        case GLFW_KEY_F7:
            invokeInput(InputContext(f7, pressed));
            break;
        case GLFW_KEY_F8:
            invokeInput(InputContext(f8, pressed));
            break;
        case GLFW_KEY_F9:
            invokeInput(InputContext(f9, pressed));
            break;
        case GLFW_KEY_F10:
            invokeInput(InputContext(f10, pressed));
            break;
        case GLFW_KEY_F11:
            invokeInput(InputContext(f11, pressed));
            break;
        case GLFW_KEY_F12:
            invokeInput(InputContext(f12, pressed));
            break;
        case GLFW_KEY_GRAVE_ACCENT:
            invokeInput(InputContext(backtick, pressed));
            break;
        case GLFW_KEY_1:
            invokeInput(InputContext(one, pressed));
            break;
        case GLFW_KEY_2:
            invokeInput(InputContext(two, pressed));
            break;
        case GLFW_KEY_3:
            invokeInput(InputContext(three, pressed));
            break;
        case GLFW_KEY_4:
            invokeInput(InputContext(four, pressed));
            break;
        case GLFW_KEY_5:
            invokeInput(InputContext(five, pressed));
            break;
        case GLFW_KEY_6:
            invokeInput(InputContext(six, pressed));
            break;
        case GLFW_KEY_7:
            invokeInput(InputContext(seven, pressed));
            break;
        case GLFW_KEY_8:
            invokeInput(InputContext(eight, pressed));
            break;
        case GLFW_KEY_9:
            invokeInput(InputContext(nine, pressed));
            break;
        case GLFW_KEY_0:
            invokeInput(InputContext(zero, pressed));
            break;
        case GLFW_KEY_MINUS:
            invokeInput(InputContext(hyphen, pressed));
            break;
        case GLFW_KEY_EQUAL:
            invokeInput(InputContext(equals, pressed));
            break;
        case GLFW_KEY_BACKSPACE:
            invokeInput(InputContext(backspace, pressed));
            break;
        case GLFW_KEY_TAB:
            invokeInput(InputContext(tab, pressed));
            break;
        case GLFW_KEY_Q:
            invokeInput(InputContext(q, pressed));
            break;
        case GLFW_KEY_W:
            invokeInput(InputContext(w, pressed));
            break;
        case GLFW_KEY_E:
            invokeInput(InputContext(e, pressed));
            break;
        case GLFW_KEY_R:
            invokeInput(InputContext(r, pressed));
            break;
        case GLFW_KEY_T:
            invokeInput(InputContext(t, pressed));
            break;
        case GLFW_KEY_Y:
            invokeInput(InputContext(y, pressed));
            break;
            break;
        case GLFW_KEY_U:
            invokeInput(InputContext(u, pressed));
            break;
            break;
        case GLFW_KEY_I:
            invokeInput(InputContext(i, pressed));
            break;
            break;
        case GLFW_KEY_O:
            invokeInput(InputContext(o, pressed));
            break;
            break;
        case GLFW_KEY_P:
            invokeInput(InputContext(p, pressed));
            break;
        case GLFW_KEY_LEFT_BRACKET:
            invokeInput(InputContext(left_bracket, pressed));
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            invokeInput(InputContext(right_bracket, pressed));
            break;
        case GLFW_KEY_BACKSLASH:
            invokeInput(InputContext(back_slash, pressed));
            break;
        case GLFW_KEY_CAPS_LOCK:
            invokeInput(InputContext(caps_lock, pressed));
            break;
        case GLFW_KEY_A:
            invokeInput(InputContext(a, pressed));
            break;
        case GLFW_KEY_S:
            invokeInput(InputContext(s, pressed));
            break;
        case GLFW_KEY_D:
            invokeInput(InputContext(d, pressed));
            break;
        case GLFW_KEY_F:
            invokeInput(InputContext(f, pressed));
            break;
        case GLFW_KEY_G:
            invokeInput(InputContext(g, pressed));
            break;
        case GLFW_KEY_H:
            invokeInput(InputContext(h, pressed));
            break;
        case GLFW_KEY_J:
            invokeInput(InputContext(j, pressed));
            break;
        case GLFW_KEY_K:
            invokeInput(InputContext(k, pressed));
            break;
        case GLFW_KEY_L:
            invokeInput(InputContext(l, pressed));
            break;
        case GLFW_KEY_SEMICOLON:
            invokeInput(InputContext(semicolon, pressed));
            break;
        case GLFW_KEY_APOSTROPHE:
            invokeInput(InputContext(apostrophe, pressed));
            break;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
            invokeInput(InputContext(enter, pressed));
            break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            invokeInput(InputContext(shift, pressed));
            break;
        case GLFW_KEY_Z:
            invokeInput(InputContext(z, pressed));
            break;
        case GLFW_KEY_X:
            invokeInput(InputContext(x, pressed));
            break;
        case GLFW_KEY_C:
            invokeInput(InputContext(c, pressed));
            break;
        case GLFW_KEY_V:
            invokeInput(InputContext(v, pressed));
            break;
        case GLFW_KEY_B:
            invokeInput(InputContext(b, pressed));
            break;
        case GLFW_KEY_N:
            invokeInput(InputContext(n, pressed));
            break;
        case GLFW_KEY_M:
            invokeInput(InputContext(m, pressed));
            break;
        case GLFW_KEY_COMMA:
            invokeInput(InputContext(comma, pressed));
            break;
        case GLFW_KEY_PERIOD:
            invokeInput(InputContext(period, pressed));
            break;
        case GLFW_KEY_SLASH:
            invokeInput(InputContext(forward_slash, pressed));
            break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            invokeInput(InputContext(control, pressed));
            break;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            invokeInput(InputContext(alt, pressed));
            break;
        case GLFW_KEY_RIGHT_SUPER:
        case GLFW_KEY_LEFT_SUPER:
            invokeInput(InputContext(super, pressed));
            break;
        case GLFW_KEY_SPACE:
            invokeInput(InputContext(space, pressed));
            break;
        case GLFW_KEY_UP:
            invokeInput(InputContext(up, pressed));
            break;
        case GLFW_KEY_DOWN:
            invokeInput(InputContext(down, pressed));
            break;
        case GLFW_KEY_RIGHT:
            invokeInput(InputContext(right, pressed));
            break;
        case GLFW_KEY_LEFT:
            invokeInput(InputContext(left, pressed));
            break;
        default:
            break;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glViewport(0, 0, width, height);
    projection = projection.ortho(-static_cast<float>(width) / 2.0f, static_cast<float>(width) / 2.0f, -static_cast<float>(height) / 2.0f, static_cast<float>(height) / 2.0f, 1, -1000);
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
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // enable the color vertexAttribute
    glEnableVertexAttribArray(1);
    // define the texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, uv));
    // enable the texture attribute
    glEnableVertexAttribArray(2);

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

void processGamepadEvents(const GLFWgamepadstate& current, const GLFWgamepadstate& last) {
    for (int i = 0; i < 14; i++) {
        // if the button state changed
        if (current.buttons[i] != last.buttons[i]) {
            const bool currentState = current.buttons[i] && !last.buttons[i];

            switch (i) {
                case GLFW_GAMEPAD_BUTTON_A:
                    invokeInput(InputContext(gamepad_south, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    invokeInput(InputContext(gamepad_east, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    invokeInput(InputContext(gamepad_west, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_Y:
                    invokeInput(InputContext(gamepad_north, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    invokeInput(InputContext(dpad_up, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    invokeInput(InputContext(dpad_down, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    invokeInput(InputContext(dpad_right, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_BACK:
                    invokeInput(InputContext(gamepad_view, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    invokeInput(InputContext(gamepad_menu, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    invokeInput(InputContext(gamepad_guide, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    invokeInput(InputContext(left_bumper, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    invokeInput(InputContext(right_bumper, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                    invokeInput(InputContext(left_stick, currentState));
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                    invokeInput(InputContext(right_stick, currentState));
                    break;
                default:
                    break;
            }
        }
    }

    // get all datas
    const float curLeftX = current.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
    const float curLeftY = current.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

    const float curRightX = current.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    const float curRightY = current.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    const float curLeftTrigger = current.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    const float curRightTrigger = current.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

    const float lastLeftX = last.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
    const float lastLeftY = last.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

    const float lastRightX = last.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    const float lastRightY = last.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    const float lastLeftTrigger = last.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    const float lastRightTrigger = last.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

    constexpr float CHANGE_THRESHOLD = 0.1f;

    // if there was change on the left stick
    if (std::abs(curLeftX - lastLeftX) > CHANGE_THRESHOLD || std::abs(curLeftY - lastLeftY) > CHANGE_THRESHOLD) {
        invokeInput(InputContext(left_stick_moved, {curLeftX, curLeftY}));
    }

    // if there was change on the right stick
    if (std::abs(curRightX - lastRightX) > CHANGE_THRESHOLD || std::abs(curRightY - lastRightY) > CHANGE_THRESHOLD) {
        invokeInput(InputContext(right_stick_moved, {curRightX, curRightY}));
    }

    // if there was a change on the left trigger
    if (std::abs(curLeftTrigger - lastLeftTrigger) > CHANGE_THRESHOLD) {
        invokeInput(InputContext(left_trigger, curLeftTrigger));
    }

    // if there was a change on the right trigger
    if (std::abs(curRightTrigger - lastRightTrigger) > CHANGE_THRESHOLD) {
        invokeInput(InputContext(right_trigger, curRightTrigger));
    }
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

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learn-opengl", nullptr, nullptr);
    if (!window)
    {
        std::printf("window creation failed");
        exit(0);
    }

    // lmk when something changes
    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // were gonna use this window rn
    glfwMakeContextCurrent(window);

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
        {{212.5, 108}, {1, 1, 1, 1}, {1, 1}}, // top right
        {{-212.5, 108}, {1, 1, 1, 1}, {0, 1}}, // top left
        {{-212.5, -108}, {1, 1, 1, 1}, {0, 0}}, // bottom left
        {{212.5, -108}, {1, 1, 1, 1}, {1, 0}} // bottom right
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Object square(vertices, indices, shader, {{0, 0}, 0, {1, 1}});
    square.Define(GL_STATIC_DRAW, "/Users/ricardito/CLionProjects/OpenGL/res/textures/dvdvd.jpg", true, GL_REPEAT);
    square.AddComponent<Moving>();
    // empty the buffers to make sure its drawing properly
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(0);

    Camera camera({0, 0}, 0);

    framebuffer_size_callback(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    auto lastLoopTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;

    auto lastFixedUpdateTime = std::chrono::high_resolution_clock::now();

    GLFWgamepadstate lastGamepadState;

    // main update loop
    while (!glfwWindowShouldClose(window)) {
        auto startOfLoopTime = std::chrono::high_resolution_clock::now();
        // calculate deltaTime
        std::chrono::duration<double, std::milli> timeSinceLastUpdateInMilli = startOfLoopTime - lastLoopTime;
        double deltaTime = timeSinceLastUpdateInMilli.count();
        lastLoopTime = std::chrono::high_resolution_clock::now();

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

        for (Object* object: allObjects) {
            object->Draw(GL_TRIANGLES, camera.viewMatrix, projection);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // framerate capping
        auto endOfLoopTime = std::chrono::high_resolution_clock::now();
        auto updateTime = std::chrono::duration<double, std::milli>(endOfLoopTime - startOfLoopTime).count();
        auto targetFrameTimeMs = 1000.0/fps;

        auto timeToSleepMs = targetFrameTimeMs - updateTime;

        if (timeToSleepMs > 0)
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(timeToSleepMs));
    }

    // clean uo things
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    // and exit
    exit(EXIT_SUCCESS);
}