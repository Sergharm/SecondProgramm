#pragma once

typedef unsigned long long uint;
typedef long long int64;  // Добавляем знаковый тип

// Расширенный алгоритм Евклида
uint extGCD(uint a, uint b, int64 &kofU, int64 &kofV);

// Поиск обратного числа по модулю
uint modInverse(uint chisloC, uint modulM);