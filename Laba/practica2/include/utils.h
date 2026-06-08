#pragma once
#include <string>

typedef unsigned long long uint;

void clear_input();

uint get_ll(const std::string& podskazka, uint minZnachenie = 0, uint maxZnachenie = 1000000000000ULL);

std::string get_str(const std::string& podskazka);

std::string get_existing_file(const std::string& podskazka);

std::string get_new_file(const std::string& podskazka);