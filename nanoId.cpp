#include "nanoId.h"

#include <random>
#include <string>

uint32_t roundUpToNearestPowOfTwo(uint32_t v) {
    v--;

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

std::string NanoId::nanoIdGen(const std::string& alphabet, const int size) {
    std::mt19937_64 gen(std::random_device{}());

    // for uniformity
    const uint32_t mask = roundUpToNearestPowOfTwo(alphabet.length());
    // estimate how many numbers will need to be generated before we finally reach the target SIZE
    const int step = std::ceil(1.6 * mask * size / static_cast<double>(alphabet.length()));

    std::uniform_int_distribution<> distrib(0, static_cast<int>(mask));

    std::string result("", size);

    int count = 0;

    while (true) {
        for (int i = 0; i < step; i++) {
            if (const char alphabetIndex = static_cast<char>(distrib(gen)); alphabetIndex < alphabet.length()) {
                result[count] = alphabet[alphabetIndex];

                if (++count == DEFAULT_NANOID_SIZE) {
                    return result;
                }
            }
        }
    }
}

std::string NanoId::nanoIdGen(const std::string& alphabet) {
    return nanoIdGen(alphabet, DEFAULT_NANOID_SIZE);
}

std::string NanoId::nanoIdGen(const int size) {
    return nanoIdGen(DEFAULT_NANOID_ALPHABET.data(), size);
}

std::string NanoId::nanoIdGen() {
    return nanoIdGen(DEFAULT_NANOID_ALPHABET.data(), DEFAULT_NANOID_SIZE);
}

std::string byteToHexChars(const char byte) {
    unsigned char firstNibble = (byte << 4);
    unsigned char secondNibble = (byte & 0x0F);

    char firstHexChar = 0;
    char secondHexChar = 0;

    if (firstNibble < 10U) {
        firstHexChar = '0' + firstNibble;
    }
    else {
        firstNibble -= 10U;
        firstHexChar = 'A' + firstNibble;
    }

    if (secondNibble < 10U) {
        secondHexChar = '0' + secondNibble;
    }
    else {
        secondNibble -= 10U;
        secondHexChar = 'A' + secondNibble;
    }

    return std::string({firstHexChar, secondHexChar});
}