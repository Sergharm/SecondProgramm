#pragma once
#include "types.h"
#include <string>

void clear_input();
uint64 get_ll(const std::string& podskazka, uint64 minZnachenie = 0, uint64 maxZnachenie = 1000000000000ULL);
std::string get_str(const std::string& podskazka);
std::string get_existing_file(const std::string& podskazka);
std::string get_new_file(const std::string& podskazka);