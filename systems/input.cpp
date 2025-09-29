#include "input.h"

#include "../object.h"
#include "../settings.h"

#include "GLFW/glfw3.h"

void cursorMoveCallback(GLFWwindow *window, const double xpos, const double ypos) {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);
    mouseMovedEvent->invoke({x, y});
}

void mouseButtonCallback(GLFWwindow *window, const int button, const int action, int mods) {
    const bool pressed = action == GLFW_PRESS;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            leftClickEvent->invoke(pressed);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            rightClickEvent->invoke(pressed);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            middleClickEvent->invoke(pressed);
            break;
        default:
            break;
    }
}

void scrollCallback(GLFWwindow *window, const double xoffset, const double yoffset) {
    const auto x = static_cast<float>(xoffset);
    const auto y = static_cast<float>(yoffset);
    scrollXEvent->invoke(x);
    scrollYEvent->invoke(y);
}

void keyCallback(GLFWwindow *window, const int key, int scancode, const int action, int mods) {
    if (action == GLFW_REPEAT) {
        return;
    }
    const bool pressed = (action == GLFW_PRESS);

    switch (key) {
        case GLFW_KEY_ESCAPE:
            escEvent->invoke(pressed);
            break;
        case GLFW_KEY_F1:
            f1Event->invoke(pressed);
            break;
        case GLFW_KEY_F2:
            f2Event->invoke(pressed);
            break;
        case GLFW_KEY_F3:
            f3Event->invoke(pressed);
            break;
        case GLFW_KEY_F4:
            f4Event->invoke(pressed);
            break;
        case GLFW_KEY_F5:
            f5Event->invoke(pressed);
            break;
        case GLFW_KEY_F6:
            f6Event->invoke(pressed);
            break;
        case GLFW_KEY_F7:
            f7Event->invoke(pressed);
            break;
        case GLFW_KEY_F8:
            f8Event->invoke(pressed);
            break;
        case GLFW_KEY_F9:
            f9Event->invoke(pressed);
            break;
        case GLFW_KEY_F10:
            f10Event->invoke(pressed);
            break;
        case GLFW_KEY_F11:
            f11Event->invoke(pressed);
            break;
        case GLFW_KEY_F12:
            f12Event->invoke(pressed);
            break;
        case GLFW_KEY_GRAVE_ACCENT:
            backtickEvent->invoke(pressed);
            break;
        case GLFW_KEY_1:
            oneEvent->invoke(pressed);
            break;
        case GLFW_KEY_2:
            twoEvent->invoke(pressed);
            break;
        case GLFW_KEY_3:
            threeEvent->invoke(pressed);
            break;
        case GLFW_KEY_4:
            fourEvent->invoke(pressed);
            break;
        case GLFW_KEY_5:
            fiveEvent->invoke(pressed);
            break;
        case GLFW_KEY_6:
            sixEvent->invoke(pressed);
            break;
        case GLFW_KEY_7:
            sevenEvent->invoke(pressed);
            break;
        case GLFW_KEY_8:
            eightEvent->invoke(pressed);
            break;
        case GLFW_KEY_9:
            nineEvent->invoke(pressed);
            break;
        case GLFW_KEY_0:
            zeroEvent->invoke(pressed);
            break;
        case GLFW_KEY_MINUS:
            hyphenEvent->invoke(pressed);
            break;
        case GLFW_KEY_EQUAL:
            equalsEvent->invoke(pressed);
            break;
        case GLFW_KEY_BACKSPACE:
            backspaceEvent->invoke(pressed);
            break;
        case GLFW_KEY_TAB:
            tabEvent->invoke(pressed);
            break;
        case GLFW_KEY_Q:
            qEvent->invoke(pressed);
            break;
        case GLFW_KEY_W:
            wEvent->invoke(pressed);
            break;
        case GLFW_KEY_E:
            eEvent->invoke(pressed);
            break;
        case GLFW_KEY_R:
            rEvent->invoke(pressed);
            break;
        case GLFW_KEY_T:
            tEvent->invoke(pressed);
            break;
        case GLFW_KEY_Y:
            yEvent->invoke(pressed);
            break;
        case GLFW_KEY_U:
            uEvent->invoke(pressed);
            break;
        case GLFW_KEY_I:
            iEvent->invoke(pressed);
            break;
        case GLFW_KEY_O:
            oEvent->invoke(pressed);
            break;
        case GLFW_KEY_P:
            pEvent->invoke(pressed);
            break;
        case GLFW_KEY_LEFT_BRACKET:
            leftBracketEvent->invoke(pressed);
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            rightBracketEvent->invoke(pressed);
            break;
        case GLFW_KEY_BACKSLASH:
            backSlashEvent->invoke(pressed);
            break;
        case GLFW_KEY_CAPS_LOCK:
            capsLockEvent->invoke(pressed);
            break;
        case GLFW_KEY_A:
            aEvent->invoke(pressed);
            break;
        case GLFW_KEY_S:
            sEvent->invoke(pressed);
            break;
        case GLFW_KEY_D:
            dEvent->invoke(pressed);
            break;
        case GLFW_KEY_F:
            fEvent->invoke(pressed);
            break;
        case GLFW_KEY_G:
            gEvent->invoke(pressed);
            break;
        case GLFW_KEY_H:
            hEvent->invoke(pressed);
            break;
        case GLFW_KEY_J:
            jEvent->invoke(pressed);
            break;
        case GLFW_KEY_K:
            kEvent->invoke(pressed);
            break;
        case GLFW_KEY_L:
            lEvent->invoke(pressed);
            break;
        case GLFW_KEY_SEMICOLON:
            semicolonEvent->invoke(pressed);
            break;
        case GLFW_KEY_APOSTROPHE:
            apostropheEvent->invoke(pressed);
            break;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
            enterEvent->invoke(pressed);
            break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            shiftEvent->invoke(pressed);
            break;
        case GLFW_KEY_Z:
            zEvent->invoke(pressed);
            break;
        case GLFW_KEY_X:
            xEvent->invoke(pressed);
            break;
        case GLFW_KEY_C:
            cEvent->invoke(pressed);
            break;
        case GLFW_KEY_V:
            vEvent->invoke(pressed);
            break;
        case GLFW_KEY_B:
            bEvent->invoke(pressed);
            break;
        case GLFW_KEY_N:
            nEvent->invoke(pressed);
            break;
        case GLFW_KEY_M:
            mEvent->invoke(pressed);
            break;
        case GLFW_KEY_COMMA:
            commaEvent->invoke(pressed);
            break;
        case GLFW_KEY_PERIOD:
            periodEvent->invoke(pressed);
            break;
        case GLFW_KEY_SLASH:
            forwardSlashEvent->invoke(pressed);
            break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            controlEvent->invoke(pressed);
            break;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            altEvent->invoke(pressed);
            break;
        case GLFW_KEY_RIGHT_SUPER:
        case GLFW_KEY_LEFT_SUPER:
            superEvent->invoke(pressed);
            break;
        case GLFW_KEY_SPACE:
            spaceEvent->invoke(pressed);
            break;
        case GLFW_KEY_UP:
            upEvent->invoke(pressed);
            break;
        case GLFW_KEY_DOWN:
            downEvent->invoke(pressed);
            break;
        case GLFW_KEY_RIGHT:
            rightEvent->invoke(pressed);
            break;
        case GLFW_KEY_LEFT:
            leftEvent->invoke(pressed);
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
                    gamepadSouthEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    gamepadEastEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    gamepadWestEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_Y:
                    gamepadNorthEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    dpadUpEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    dpadDownEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    dpadRightEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_BACK:
                    gamepadViewEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    gamepadMenuEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    gamepadGuideEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    leftBumperEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    rightBumperEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                    leftStickEvent->invoke(currentState);
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                    rightStickEvent->invoke(currentState);
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
        leftStickMovedEvent->invoke({curLeftX, curLeftY});
    }

    // if there was change on the right stick
    if (std::abs(curRightX - lastRightX) > CHANGE_THRESHOLD || std::abs(curRightY - lastRightY) > CHANGE_THRESHOLD) {
        rightStickMovedEvent->invoke({curRightX, curRightY});
    }

    // if there was a change on the left trigger
    if (std::abs(curLeftTrigger - lastLeftTrigger) > CHANGE_THRESHOLD) {
        leftTriggerEvent->invoke(curLeftTrigger);
    }

    // if there was a change on the right trigger
    if (std::abs(curRightTrigger - lastRightTrigger) > CHANGE_THRESHOLD) {
        rightTriggerEvent->invoke(curRightTrigger); // NOTE: Fixed a potential typo here, it was previously invoking left_trigger_event for the right trigger.
    }
}