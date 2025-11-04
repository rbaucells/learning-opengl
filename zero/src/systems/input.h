#pragma once
#include <memory>

#include "events.h"
#include "math++/math.h"

// forward declaration of GLFW things
struct GLFWwindow;
struct GLFWgamepadstate;

void cursorMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void processGamepadEvents(const GLFWgamepadstate& current, const GLFWgamepadstate& last);

Vector<2> mousePosToScreenPos(const Vector<2>& mousePos);

inline std::shared_ptr<Publisher<bool>> escEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f1Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f2Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f3Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f4Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f5Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f6Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f7Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f8Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f9Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f10Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f11Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> f12Event = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> backtickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> oneEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> twoEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> threeEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> fourEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> fiveEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> sixEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> sevenEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> eightEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> nineEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> zeroEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> hyphenEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> equalsEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> backspaceEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> tabEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> qEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> wEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> eEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> tEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> yEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> uEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> iEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> oEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> pEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> leftBracketEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rightBracketEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> backSlashEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> capsLockEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> aEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> sEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> dEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> fEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> hEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> jEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> kEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> lEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> semicolonEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> apostropheEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> enterEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> shiftEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> zEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> xEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> cEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> vEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> bEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> nEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> mEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> commaEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> periodEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> forwardSlashEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> controlEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> altEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> superEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> spaceEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> leftEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> upEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> downEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rightEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> leftClickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rightClickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> middleClickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadSouthEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadEastEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadWestEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadNorthEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> leftBumperEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rightBumperEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> leftStickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> rightStickEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> dpadUpEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> dpadDownEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> dpadRightEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> dpadLeftEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadViewEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadMenuEvent = Publisher<bool>::create();
inline std::shared_ptr<Publisher<bool>> gamepadGuideEvent = Publisher<bool>::create();

inline std::shared_ptr<Publisher<Vector<2>>> mouseMovedEvent = Publisher<Vector<2>>::create();
inline std::shared_ptr<Publisher<Vector<2>>> leftStickMovedEvent = Publisher<Vector<2>>::create();
inline std::shared_ptr<Publisher<Vector<2>>> rightStickMovedEvent = Publisher<Vector<2>>::create();

inline std::shared_ptr<Publisher<float>> leftTriggerEvent = Publisher<float>::create();
inline std::shared_ptr<Publisher<float>> rightTriggerEvent = Publisher<float>::create();
inline std::shared_ptr<Publisher<float>> scrollXEvent = Publisher<float>::create();
inline std::shared_ptr<Publisher<float>> scrollYEvent = Publisher<float>::create();
