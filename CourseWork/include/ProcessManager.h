#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <vector>

struct ProcessResult {
    int exitCode;
    bool timedOut;
    std::string output;
};

class ProcessManager {
public:
    ProcessResult run(const std::vector<std::string>& command, std::string_view program,
                      std::chrono::milliseconds timeout) const;
};
