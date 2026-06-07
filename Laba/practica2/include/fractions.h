#ifndef FRACTIONS_H
#define FRACTIONS_H

#include <vector>
using namespace std;

// цепная дробь
vector<long long> chain_fraction(long long num, long long den);

// решение диофантова уравнения
void solve_diofant(long long A, long long B, long long D);

#endif