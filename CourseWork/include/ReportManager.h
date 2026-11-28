#pragma once

#include <filesystem>
#include <string>

class ReportManager {
public:
    std::filesystem::path save(const std::string& text) const;
};
