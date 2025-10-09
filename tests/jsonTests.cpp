#include <filesystem>
#include <iostream>
#include <string>
#include <__filesystem/filesystem_error.h>
#include "../json/jsonValue.h"

#include "../json/jsonLexer.h"
#include "../json/jsonParser.h"

constexpr char TESTS_TO_RUN = 'n';

int main() {
    const std::string directoryPath = "/Users/ricardito/Projects/learning-opengl/res/json files/test suite";

    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        std::cerr << "Error: Folder \'" << directoryPath << "\' does not exist" << std::endl;
        return 1;
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            const std::string fileName = entry.path().filename().string();

            if (fileName.at(0) == 'i' && 'i' == TESTS_TO_RUN) {
                try {
                    std::ifstream inputStream(entry.path());
                    JsonLexer lexer(inputStream);
                    JsonParser parser(lexer);
                    auto value = parser.parseValue();
                }
                catch (std::exception& e) {
                    std::cout << "Failed test \'" << fileName << "\' but we don't really care - ✅\n";
                    continue;
                }

                std::cout << "Passed test \'" << fileName << "\' but we don't really care - ✅\n";
            }
            else if (fileName.at(0) == 'y' && 'y' == TESTS_TO_RUN) {
                // try {
                std::ifstream inputStream(entry.path());
                JsonLexer lexer(inputStream);
                JsonParser parser(lexer);
                auto value = parser.parseValue();
                // }
                // catch (std::exception& e) {
                // std::cout << "Failed test \'" << fileName << "\' - ❌\n";
                // continue;
                // }
                std::cout << "Passed test \'" << fileName << "\' - ✅\n";
            }
            else if (fileName.at(0) == 'n' && 'n' == TESTS_TO_RUN) {
                try {
                    std::ifstream inputStream(entry.path());
                    JsonLexer lexer(inputStream);
                    JsonParser parser(lexer);
                    auto value = parser.parseValue();
                }
                catch (std::exception& e) {
                    std::cout << "Failed test \'" << fileName << "\' but we thats expected - ✅\n";
                    continue;
                }

                std::cout << "Passed test \'" << fileName << "\' but we werent supposed to - ❌\n";
            }
        }
    }
}