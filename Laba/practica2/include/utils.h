#pragma once
#include <cstdint>
#include <string>

void clear_input();

// Значения по умолчанию указываем ТОЛЬКО здесь
uint64_t get_ll(const std::string& podskazka, uint64_t minZnachenie = 0, uint64_t maxZnachenie = 1000000000000ULL);

std::string get_str(const std::string& podskazka);

std::string get_existing_file(const std::string& podskazka);

std::string get_new_file(const std::string& podskazka);