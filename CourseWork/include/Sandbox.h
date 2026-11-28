#pragma once

#include "Event.h"
#include "IsolationManager.h"
#include "NetworkMonitor.h"
#include "ProcessManager.h"
#include "ReportManager.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct FileInfo {
    std::filesystem::path path;
    std::uintmax_t size;
    bool executable;
    std::string type;
};

struct Analysis {
    std::string status;
    std::vector<Event> events;
    std::string diagnostics;
};

class Sandbox {
public:
    explicit Sandbox(std::filesystem::path binaryDirectory);
    void select(const std::filesystem::path& path);
    const std::filesystem::path& selected() const;
    FileInfo fileInfo() const;
    const Analysis& analyze();
    const std::optional<Analysis>& lastAnalysis() const;
    const std::vector<std::string>& journal() const;
    std::filesystem::path saveReport();

private:
    void log(const std::string& message);

    std::filesystem::path binaryDirectory_;
    std::filesystem::path selected_;
    std::optional<Analysis> lastAnalysis_;
    std::vector<std::string> journal_;
    ProcessManager process_;
    IsolationManager isolation_;
    NetworkMonitor monitor_;
    ReportManager reports_;
};
