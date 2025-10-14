#pragma once
#include <string>

namespace NanoId {
    constexpr int DEFAULT_NANOID_SIZE = 21;
    constexpr std::string_view DEFAULT_NANOID_ALPHABET = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";

    std::string nanoIdGen(const std::string& alphabet, int size);
    std::string nanoIdGen(const std::string& alphabet);
    std::string nanoIdGen(int size);
    std::string nanoIdGen();
}