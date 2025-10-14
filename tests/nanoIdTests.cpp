#include <iostream>
#include <ranges>
#include <regex>

#include "../nanoId.h"

void testDistribution() {
    // character with how many times its popped up
    std::unordered_map<char, int> occurances;

    for (int i = 0; i < 100000; i++) {
        for (const std::string nanoId = NanoId::nanoIdGen(); const char c : nanoId) {
            if (occurances.contains(c)) {
                occurances[c]++;
            }
            else {
                occurances[c] = 1;
            }
        }
    }

    double avgOccurances = 0;

    for (const auto& value : occurances | std::views::values) {
        avgOccurances += value;
    }

    avgOccurances /= occurances.size();

    std::cout << "Average Occurences: " << avgOccurances << std::endl;

    double meanAbsDeviation = 0.0;

    for (const auto& value : occurances | std::views::values) {
        meanAbsDeviation += std::abs(static_cast<double>(value) - avgOccurances);
    }

    meanAbsDeviation /= occurances.size();

    std::cout << "Mean Absolute Deviation: " << meanAbsDeviation << std::endl;
}

int main() {
    testDistribution();
    return 0;
}
