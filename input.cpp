#include "input.h"

#include "main.h"
#include "object.h"

#include "GLFW/glfw3.h"

void invokeInput(const InputContext &context) {
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

void cursor_move_callback(GLFWwindow *window, double xpos, double ypos)  {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);
    invokeInput(InputContext(mouse_moved, {x, y}));
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
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

vector2 mousePosToScreenPos(const vector2& mousePos) {
    float x = mousePos.x - SCREEN_WIDTH * 0.5f;
    float y = SCREEN_HEIGHT * 0.5f - mousePos.y;
    return vector2(x, y);
}
