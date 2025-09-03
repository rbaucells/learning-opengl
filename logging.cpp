#include "logging.h"

#include <iostream>

Logging::Logging(const std::string &funcName) {
    this->name = funcName;
    startTimePoint = std::chrono::high_resolution_clock::now();
    std::cout << funcName << " has started \n";
}

Logging::~Logging() {
    float time = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - startTimePoint).count();
    std::cout << name << " has finished. Took \"" << time << "\" seconds \n";
}