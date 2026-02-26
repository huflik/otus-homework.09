#pragma once

#include <vector>
#include <string>
#include <ctime>

struct Bulk {
    Bulk() = default;
        
    std::vector<std::string> commands;
    std::time_t timestamp{};
};