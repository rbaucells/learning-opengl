#include "idGenerators.h"

#include <random>
#include <string>

constexpr int SIZE = 21;
uint32_t roundUpToNearestPowOfTwo(uint32_t v)
{
    v--;

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

std::string IdGenerators::nanoIdGen() {
    constexpr char ALLOWED[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    constexpr int length = 63;

    std::mt19937 gen(std::random_device{}());

    // for uniformity
    const int mask = roundUpToNearestPowOfTwo(length);
    // estimate how many numbers will need to be generated before we finnaly reach the target SIZE
    int step = std::ceil(1.6 * mask * SIZE / length);

    std::uniform_int_distribution<> distrib(0, mask);

    std::string result("", SIZE);

    int count = 0;

    while (true) {
        for (int i = 0; i < step; i++) {
            char alphabetIndex = distrib(gen);

            if (alphabetIndex >= length)
                continue;
            else {
                result.at(count) = ALLOWED[alphabetIndex];
                if (++count == SIZE)
                {
                    return result;
                }
            }
        }
    }
}