#pragma once

#include <string>

struct Event {
    int pid;
    std::string call;
    std::string protocol;
    std::string address;
    std::string port;
    std::string result;
    bool success;
};
