#pragma once
#include <cstdint>

// Проверка числа на простоту
bool isPrime(uint64_t chisloN);

// Обычный алгоритм Евклида для поиска НОД
uint64_t gcd(uint64_t a, uint64_t b);

// Быстрое бинарное возведение в степень по модулю
uint64_t powerMod(uint64_t baza, uint64_t stepen, uint64_t modul, bool show_log = true);

// Возведение в степень по теореме Ферма
uint64_t powerModFermat(uint64_t baza, uint64_t stepen, uint64_t modul, bool show_log = true);