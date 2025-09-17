// #pragma once
// #include <string>
// #include <vector>
//
// #include "texture.h"
// #include "../transform.h"
// #include "../math/Color.h"
//
// class Object;
//
// class Text final : public Component {
// public:
//     enum Alignment {
//         left,
//         middle,
//         right
//     };
//
//     Text(Object* owner, const std::string& text, const std::string& fontPath, int fontSize, Color color = {1, 1, 1, 1}, Alignment alignment = middle);
//
// private:
//     std::string text_;
//     Texture fontTexture_;
//     int fontSize_;
//     Color color_ = {};
//     Alignment alignment_;
//
//     std::vector<std::unique_ptr<Object>> textObjects_;
// };
