#pragma once

// Проверка числа на простоту
bool isPrime(long long n);

// Обычный алгоритм Евклида для поиска НОД
long long gcd(long long a, long long b);

// Быстрое бинарное возведение в степень по модулю (добавлен переключатель логов)
long long powerMod(long long base, long long exp, long long mod, bool show_log = true);

// Возведение в степень по теореме Ферма (добавлен переключатель логов)
long long powerModFermat(long long base, long long exp, long long mod, bool show_log = true);
