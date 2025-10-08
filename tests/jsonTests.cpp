#include <cassert>
#include <iostream>
#include <sstream>

#include "../json/jsonLexer.h"
#include "../json/jsonParser.h"
#include "../json/jsonToken.h"
#include "../json/jsonValue.h"

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

        auto expect = [&](JsonToken::Type type, const std::string& value = "") {
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

        expect(JsonToken::Type::string, "Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, "12.345");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "Negative Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, "-2025.10");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "String");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, "The quick brown fox jumps over the lazy dog");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "String with escaped literals");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, "\u20AC <- Euro 'Something in the middle' Check Mark -> \u2705");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "Boolean");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::boolean, "true");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "Null");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::null, "null");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "Array");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::start_array);

        expect(JsonToken::Type::number, "1");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "2");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "3");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "4");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "5");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "6");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "7");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "8");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::number, "9");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::string, "a");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, "b");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, "c");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::boolean, "true");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::boolean, "false");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::null, "null");
        expect(JsonToken::Type::comma);

        expect(JsonToken::Type::start_object);
        expect(JsonToken::Type::string, "String inside an object inside an array inside an object");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::string, "hi");
        expect(JsonToken::Type::comma);
        expect(JsonToken::Type::string, "Another Number");
        expect(JsonToken::Type::colon);
        expect(JsonToken::Type::number, "100e-2");
        expect(JsonToken::Type::end_object);

        expect(JsonToken::Type::end_array);
        expect(JsonToken::Type::end_object);
        expect(JsonToken::Type::eof);

        std::cout << "✅ JSON lexer test passed with " << tokens.size() << " tokens.\n";
    }

    {
        std::ifstream fileStream("/Users/ricardito/Projects/learning-opengl/res/json files/types.json");

        JsonLexer lexer(fileStream);
        JsonParser parser(lexer);

        JsonValue value = parser.parseValue();

        if (value.type == JsonValue::Type::object) {
            JsonObject obj = value;

            obj.putNumberField("This is a new number that i added cuz yes", 21);

            std::cout << obj.toString();
        }
    }
}
