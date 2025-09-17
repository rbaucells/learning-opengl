// #include "text.h"
//
// #include "../object.h"
// #include "glad/gl.h"
//
// Text::Text(Object* owner, const std::string& text, const std::string& fontPath, const int fontSize, const Color color, Alignment alignment) : Component(owner), fontTexture_(fontPath, GL_CLAMP, true) {
//     this->alignment_ = alignment;
//     this->fontSize_ = fontSize;
//     this->color_ = color;
//     this->text_ = text;
//
//     switch (this->alignment_) {
//         case left:
//             Vector2 lastPos = object->transform.getGlobalPosition();
//
//             for (char c : text) {
//                 if (c == '\n' || c == '\r') {
//                     lastPos.y -= fontSize_;
//                 }
//
//                 auto pos = Vector2(lastPos.x += fontSize_, lastPos.y);
//
//                 auto obj = std::make_unique<Object>(c, 100, pos, 0, {1, 1});
//                 textObjects_.push_back()
//             }
//             break;
//         case middle:
//             for (char c : text) {
//
//             }
//             break;
//         case right:
//             for (char c : text) {
//
//             }
//             break;
//         default:
//             break;
//     }
// }
