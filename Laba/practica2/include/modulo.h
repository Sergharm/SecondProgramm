#pragma once
#include "types.h"

bool isPrime(uint64 chisloN);
uint64 gcd(uint64 a, uint64 b);
uint64 powerMod(uint64 baza, uint64 stepen, uint64 modul, bool show_log = true);
uint64 powerModFermat(uint64 baza, uint64 stepen, uint64 modul, bool show_log = true);