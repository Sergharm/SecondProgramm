#pragma once
#include <string>

// Запуск консольного меню
void showMenu();

// Безопасный ввод чисел с проверкой границ
long long get_ll(const std::string& prompt, long long min_val = 0, long long max_val = 1000000000000LL);
