#pragma once
#include "math/mathematics.h"

// forward declaration of GLFW things
struct GLFWwindow;
struct GLFWgamepadstate;

enum InputAction {
    esc, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
    backtick, one, two, three, four, five, six, seven, eight, nine, zero, hyphen, equals, backspace,
    tab, q, w, e, r, t, y, u, i, o, p, left_bracket, right_bracket, back_slash,
    caps_lock, a, s, d, f, g, h, j, k, l, semicolon, apostrophe, enter,
    shift, z, x, c, v, b, n, m, comma, period, forward_slash,
    control, alt, super, space, left, up, down, right,

    left_click, right_click, middle_click, scroll_x, scroll_y,

    gamepad_south, gamepad_east, gamepad_west, gamepad_north,
    left_bumper, right_bumper,
    left_stick, right_stick,
    dpad_up, dpad_down, dpad_right, dpad_left,
    gamepad_view, gamepad_menu, gamepad_guide,

    mouse_moved, left_stick_moved, right_stick_moved, left_trigger, right_trigger
};

struct InputContext {
    InputContext(const InputAction action, const float extraFloat) {
        this -> action = action;
        this -> extraFloat = extraFloat;
    }
    InputContext(const InputAction action, const vector2 extraVector2) {
        this -> action = action;
        this -> extraVector2 = extraVector2;
    }
    InputContext(const InputAction action, const bool state) {
        this -> action = action;
        this -> state = state;
    }

    InputAction action;

    vector2 extraVector2 = {};
    float extraFloat = 0;

    bool state = false;
};

void invokeInput(const InputContext& context);

void cursor_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void processGamepadEvents(const GLFWgamepadstate& current, const GLFWgamepadstate& last);

vector2 mousePosToScreenPos(const vector2& mousePos);