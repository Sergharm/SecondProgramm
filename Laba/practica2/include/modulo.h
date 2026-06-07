#ifndef MODULO_H
#define MODULO_H

using namespace std; // ну а чё, так проще

// проверка на простоту, ну типа перебором
bool is_prime(long long chislo);

// a^x mod p через ферма (если p простое)
long long fermat_pow(long long a, long long x, long long p);

// a^x mod m через двоичное разложение (быстрое)
long long binary_pow(long long a, long long x, long long m);

// безопасное умножение чтобы не было переполнения
long long safe_mul(long long a, long long b, long long mod);

#endif