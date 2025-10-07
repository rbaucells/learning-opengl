#include <iosfwd>
#include <sstream>
#include <string>

#include "../json/jsonLexer.h"
#include "../json/jsonToken.h"

int main() {
    const std::string jsonContent = R"({
  "key1": true,
  "key2": 123,
  "key3": "hello",
  "key-4": -1.5E3,
  "list": [
    "item one",
    null,
    false,
    "Backslash:\\\\ and Quote:\"",
    "Newline\nTab\t",
    "Euro\u20AC",
    "\uD83D\uDE00"
  ]
})";

    std::stringstream ss(jsonContent);

    JsonLexer lexer(ss);

    std::vector<JsonToken> tokens;

    JsonToken token;

    while (token.type != JsonToken::Type::eof) {
        token = lexer.nextToken();
        tokens.push_back(token);
    }

    printf("Lexed\n");
}
