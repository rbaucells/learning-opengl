#pragma once
#include <string>

class Logging  {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTimePoint;
public:
    Logging(const std::string& funcName);
    ~Logging();
};
