#include <cassert>
#include <iostream>
#include <sstream>

#include "../json/jsonLexer.h"
#include "../json/jsonToken.h"

int main() {
    {
        std::ifstream fileStream("/Users/ricardito/Projects/learning-opengl/res/json files/types.json");

        JsonLexer lexer(fileStream);

        std::vector<JsonToken> tokens;

        JsonToken token;
        while (token.type != JsonToken::Type::eof) {
            token = lexer.nextToken();
            tokens.push_back(token);
        }

        // === Begin Tests ===
        size_t i = 0;

        auto expect = [&](JsonToken::Type type, const std::u8string& value = u8"") {
            assert(i < tokens.size());
            const auto& t = tokens[i++];
            if (t.type != type) {
                std::cerr << "Token mismatch at index " << (i - 1)
                    << ": expected type " << static_cast<int>(type)
                    << ", got " << static_cast<int>(t.type) << "\n";
                std::abort();
            }
            if (!value.empty() && t.value != value) {
                std::cerr << "Value mismatch at index " << (i - 1)
                    << ": expected '" << reinterpret_cast<const char*>(value.c_str())
                    << "', got '" << reinterpret_cast<const char*>(t.value.c_str()) << "'\n";
                std::abort();
            }
        };

        // === Expected Tokens ===

        expect(JsonToken::Type::start_object);

        expect(JsonToken::Type::string, u8"Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, u8"12.345");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"Negative Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, u8"-2025.10");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"String");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, u8"The quick brown fox jumps over the lazy dog");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"String with escaped literals");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, u8"\u20AC <- Euro 'Something in the middle' Check Mark -> \u2705");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"Boolean");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::boolean, u8"true");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"Null");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::null, u8"null");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"Array");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::start_array);

        expect(JsonToken::Type::number, u8"1");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"2");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"3");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"4");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"5");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"6");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"7");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"8");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, u8"9");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, u8"a");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, u8"b");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, u8"c");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::boolean, u8"true");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::boolean, u8"false");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::null, u8"null");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::start_object);
        expect(JsonToken::Type::string, u8"String inside an object inside an array inside an object");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, u8"hi");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, u8"Another Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, u8"12e-5");
        expect(JsonToken::Type::end_object);

        expect(JsonToken::Type::end_array);
        expect(JsonToken::Type::end_object);
        expect(JsonToken::Type::eof);

        std::cout << "✅ JSON lexer test passed with " << tokens.size() << " tokens.\n";
    }
}
