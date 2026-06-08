#pragma once
#include <string>

void clear_input();

long long get_ll(const std::string& prompt, long long min_val = 0, long long max_val = 1000000000000LL);

std::string get_str(const std::string& prompt);

std::string get_existing_file(const std::string& prompt);

std::string get_new_file(const std::string& prompt);
