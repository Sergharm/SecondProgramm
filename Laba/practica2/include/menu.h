#pragma once
#include <string>

typedef unsigned long long uint;

// Запуск консольного меню
void showMenu();

// Безопасный ввод чисел с проверкой границ
uint get_ll(const std::string& podskazka, uint minZnachenie = 0, uint maxZnachenie = 1000000000000ULL);