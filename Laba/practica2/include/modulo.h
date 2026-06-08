#pragma once

typedef unsigned long long uint;

// Проверка числа на простоту (метод перебора, для лабы хватит)
bool isPrime(uint chisloN);

// Обычный алгоритм Евклида для поиска НОД
uint gcd(uint a, uint b);

// Быстрое бинарное возведение в степень по модулю (добавлен переключатель логов)
uint powerMod(uint baza, uint stepen, uint modul, bool show_log = true);

// Возведение в степень по теореме Ферма (добавлен переключатель логов)
uint powerModFermat(uint baza, uint stepen, uint modul, bool show_log = true);