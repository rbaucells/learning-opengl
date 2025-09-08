#pragma once
#include "event.h"
#include "math/vector2.h"

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
        this->action = action;
        this->extraFloat = extraFloat;
    }

    InputContext(const InputAction action, const Vector2 extraVector2) {
        this->action = action;
        this->extraVector2 = extraVector2;
    }

    InputContext(const InputAction action, const bool state) {
        this->action = action;
        this->state = state;
    }

    InputAction action;

    Vector2 extraVector2 = {};
    float extraFloat = 0;

    bool state = false;
};

void invokeInput(const InputContext &context);

void cursor_move_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void processGamepadEvents(const GLFWgamepadstate &current, const GLFWgamepadstate &last);

Vector2 mousePosToScreenPos(const Vector2 &mousePos);

inline Event<bool> esc_event, f1_event, f2_event, f3_event, f4_event, f5_event, f6_event, f7_event, f8_event, f9_event, f10_event, f11_event, f12_event,
        backtick_event, one_event, two_event, three_event, four_event, five_event, six_event, seven_event, eight_event, nine_event, zero_event, hyphen_event, equals_event, backspace_event,
        tab_event, q_event, w_event, e_event, r_event, t_event, y_event, u_event, i_event, o_event, p_event, left_bracket_event, right_bracket_event, back_slash_event,
        caps_lock_event, a_event, s_event, d_event, f_event, g_event, h_event, j_event, k_event, l_event, semicolon_event, apostrophe_event, enter_event,
        shift_event, z_event, x_event, c_event, v_event, b_event, n_event, m_event, comma_event, period_event, forward_slash_event,
        control_event, alt_event, super_event, space_event, left_event, up_event, down_event, right_event,
        left_click_event, right_click_event, middle_click_event,
        gamepad_south_event, gamepad_east_event, gamepad_west_event, gamepad_north_event,
        left_bumper_event, right_bumper_event,
        left_stick_event, right_stick_event,
        dpad_up_event, dpad_down_event, dpad_right_event, dpad_left_event,
        gamepad_view_event, gamepad_menu_event, gamepad_guide_event;

inline Event<Vector2> mouse_moved_event, left_stick_moved_event, right_stick_moved_event;

inline Event<float> left_trigger_event, right_trigger_event, scroll_x_event, scroll_y_event;
