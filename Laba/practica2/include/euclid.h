#ifndef EUCLID_H
#define EUCLID_H

using namespace std;

// структура для результатов расширенного евклида
struct EuclidResult {
    long long u;
    long long v;
    long long gcd;
};

// расширенный алгоритм евклида
EuclidResult ext_gcd(long long a, long long b);

// обратное число по модулю
long long mod_inverse(long long c, long long m);

#endif