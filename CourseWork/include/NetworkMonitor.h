#pragma once

#include "Event.h"

#include <string>
#include <vector>

struct NetworkTrace {
    std::vector<Event> events;
    std::string diagnostics;
};

class NetworkMonitor {
public:
    NetworkTrace parse(const std::string& output) const;
};
