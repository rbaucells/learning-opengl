#include "input.h"

#include "main.h"
#include "object.h"

#include "GLFW/glfw3.h"

void cursor_move_callback(GLFWwindow *window, double xpos, double ypos) {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);
    mouse_moved_event.invoke({x, y});
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    bool pressed = action == GLFW_PRESS;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            left_click_event.invoke(pressed);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            right_click_event.invoke(pressed);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            middle_click_event.invoke(pressed);
            break;
        default:
            break;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    const auto x = static_cast<float>(xoffset);
    const auto y = static_cast<float>(yoffset);
    scroll_x_event.invoke(x);
    scroll_y_event.invoke(y);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) {
        return;
    }
    const bool pressed = (action == GLFW_PRESS);

    switch (key) {
        case GLFW_KEY_ESCAPE:
            esc_event.invoke(pressed);
            break;
        case GLFW_KEY_F1:
            f1_event.invoke(pressed);
            break;
        case GLFW_KEY_F2:
            f2_event.invoke(pressed);
            break;
        case GLFW_KEY_F3:
            f3_event.invoke(pressed);
            break;
        case GLFW_KEY_F4:
            f4_event.invoke(pressed);
            break;
        case GLFW_KEY_F5:
            f5_event.invoke(pressed);
            break;
        case GLFW_KEY_F6:
            f6_event.invoke(pressed);
            break;
        case GLFW_KEY_F7:
            f7_event.invoke(pressed);
            break;
        case GLFW_KEY_F8:
            f8_event.invoke(pressed);
            break;
        case GLFW_KEY_F9:
            f9_event.invoke(pressed);
            break;
        case GLFW_KEY_F10:
            f10_event.invoke(pressed);
            break;
        case GLFW_KEY_F11:
            f11_event.invoke(pressed);
            break;
        case GLFW_KEY_F12:
            f12_event.invoke(pressed);
            break;
        case GLFW_KEY_GRAVE_ACCENT:
            backtick_event.invoke(pressed);
            break;
        case GLFW_KEY_1:
            one_event.invoke(pressed);
            break;
        case GLFW_KEY_2:
            two_event.invoke(pressed);
            break;
        case GLFW_KEY_3:
            three_event.invoke(pressed);
            break;
        case GLFW_KEY_4:
            four_event.invoke(pressed);
            break;
        case GLFW_KEY_5:
            five_event.invoke(pressed);
            break;
        case GLFW_KEY_6:
            six_event.invoke(pressed);
            break;
        case GLFW_KEY_7:
            seven_event.invoke(pressed);
            break;
        case GLFW_KEY_8:
            eight_event.invoke(pressed);
            break;
        case GLFW_KEY_9:
            nine_event.invoke(pressed);
            break;
        case GLFW_KEY_0:
            zero_event.invoke(pressed);
            break;
        case GLFW_KEY_MINUS:
            hyphen_event.invoke(pressed);
            break;
        case GLFW_KEY_EQUAL:
            equals_event.invoke(pressed);
            break;
        case GLFW_KEY_BACKSPACE:
            backspace_event.invoke(pressed);
            break;
        case GLFW_KEY_TAB:
            tab_event.invoke(pressed);
            break;
        case GLFW_KEY_Q:
            q_event.invoke(pressed);
            break;
        case GLFW_KEY_W:
            w_event.invoke(pressed);
            break;
        case GLFW_KEY_E:
            e_event.invoke(pressed);
            break;
        case GLFW_KEY_R:
            r_event.invoke(pressed);
            break;
        case GLFW_KEY_T:
            t_event.invoke(pressed);
            break;
        case GLFW_KEY_Y:
            y_event.invoke(pressed);
            break;
        case GLFW_KEY_U:
            u_event.invoke(pressed);
            break;
        case GLFW_KEY_I:
            i_event.invoke(pressed);
            break;
        case GLFW_KEY_O:
            o_event.invoke(pressed);
            break;
        case GLFW_KEY_P:
            p_event.invoke(pressed);
            break;
        case GLFW_KEY_LEFT_BRACKET:
            left_bracket_event.invoke(pressed);
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            right_bracket_event.invoke(pressed);
            break;
        case GLFW_KEY_BACKSLASH:
            back_slash_event.invoke(pressed);
            break;
        case GLFW_KEY_CAPS_LOCK:
            caps_lock_event.invoke(pressed);
            break;
        case GLFW_KEY_A:
            a_event.invoke(pressed);
            break;
        case GLFW_KEY_S:
            s_event.invoke(pressed);
            break;
        case GLFW_KEY_D:
            d_event.invoke(pressed);
            break;
        case GLFW_KEY_F:
            f_event.invoke(pressed);
            break;
        case GLFW_KEY_G:
            g_event.invoke(pressed);
            break;
        case GLFW_KEY_H:
            h_event.invoke(pressed);
            break;
        case GLFW_KEY_J:
            j_event.invoke(pressed);
            break;
        case GLFW_KEY_K:
            k_event.invoke(pressed);
            break;
        case GLFW_KEY_L:
            l_event.invoke(pressed);
            break;
        case GLFW_KEY_SEMICOLON:
            semicolon_event.invoke(pressed);
            break;
        case GLFW_KEY_APOSTROPHE:
            apostrophe_event.invoke(pressed);
            break;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
            enter_event.invoke(pressed);
            break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            shift_event.invoke(pressed);
            break;
        case GLFW_KEY_Z:
            z_event.invoke(pressed);
            break;
        case GLFW_KEY_X:
            x_event.invoke(pressed);
            break;
        case GLFW_KEY_C:
            c_event.invoke(pressed);
            break;
        case GLFW_KEY_V:
            v_event.invoke(pressed);
            break;
        case GLFW_KEY_B:
            b_event.invoke(pressed);
            break;
        case GLFW_KEY_N:
            n_event.invoke(pressed);
            break;
        case GLFW_KEY_M:
            m_event.invoke(pressed);
            break;
        case GLFW_KEY_COMMA:
            comma_event.invoke(pressed);
            break;
        case GLFW_KEY_PERIOD:
            period_event.invoke(pressed);
            break;
        case GLFW_KEY_SLASH:
            forward_slash_event.invoke(pressed);
            break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            control_event.invoke(pressed);
            break;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            alt_event.invoke(pressed);
            break;
        case GLFW_KEY_RIGHT_SUPER:
        case GLFW_KEY_LEFT_SUPER:
            super_event.invoke(pressed);
            break;
        case GLFW_KEY_SPACE:
            space_event.invoke(pressed);
            break;
        case GLFW_KEY_UP:
            up_event.invoke(pressed);
            break;
        case GLFW_KEY_DOWN:
            down_event.invoke(pressed);
            break;
        case GLFW_KEY_RIGHT:
            right_event.invoke(pressed);
            break;
        case GLFW_KEY_LEFT:
            left_event.invoke(pressed);
            break;
        default:
            break;
    }
}

void processGamepadEvents(const GLFWgamepadstate &current, const GLFWgamepadstate &last) {
    for (int i = 0; i < 14; i++) {
        // if the button state changed
        if (current.buttons[i] != last.buttons[i]) {
            const bool currentState = current.buttons[i] && !last.buttons[i];

            switch (i) {
                case GLFW_GAMEPAD_BUTTON_A:
                    gamepad_south_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    gamepad_east_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    gamepad_west_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_Y:
                    gamepad_north_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    dpad_up_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    dpad_down_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    dpad_right_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_BACK:
                    gamepad_view_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    gamepad_menu_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    gamepad_guide_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    left_bumper_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    right_bumper_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                    left_stick_event.invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                    right_stick_event.invoke(currentState);
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
        left_stick_moved_event.invoke({curLeftX, curLeftY});
    }

    // if there was change on the right stick
    if (std::abs(curRightX - lastRightX) > CHANGE_THRESHOLD || std::abs(curRightY - lastRightY) > CHANGE_THRESHOLD) {
        right_stick_moved_event.invoke({curRightX, lastRightX});
    }

    // if there was a change on the left trigger
    if (std::abs(curLeftTrigger - lastLeftTrigger) > CHANGE_THRESHOLD) {
        left_trigger_event.invoke(curLeftTrigger);
    }

    // if there was a change on the right trigger
    if (std::abs(curRightTrigger - lastRightTrigger) > CHANGE_THRESHOLD) {
        left_trigger_event.invoke(curRightTrigger);
    }
}
