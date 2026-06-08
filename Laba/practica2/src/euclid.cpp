// src/euclid.cpp
#include "../include/euclid.h"
#include <iostream>
#include <cstdint> 

using namespace std;

uint64_t extGCD(uint64_t a, uint64_t b, int64_t &kofU, int64_t &kofV) {
    int64_t u0 = 1, v0 = 0; 
    int64_t u1 = 0, v1 = 1; 
    
    uint64_t shag = 1;
    cout << "[Начало цикла Евклида]\n";

    while (b != 0) {
        uint64_t chastnoeQ = a / b; 
        uint64_t ostatokR = a % b; 

        int64_t nextU = u0 - (int64_t)chastnoeQ * u1;
        int64_t nextV = v0 - (int64_t)chastnoeQ * v1;

        cout << "    Шаг " << shag++ << ": " << a << " / " << b 
             << " = " << chastnoeQ << " (остаток " << ostatokR << ")\n";
        if (ostatokR != 0) {
            cout << "          Коэффициенты для остатка " << ostatokR 
                 << ": q = " << chastnoeQ 
                 << ", u = " << nextU << ", v = " << nextV << "\n";
        }

        a = b;
        b = ostatokR;

        u0 = u1; v0 = v1;
        u1 = nextU; v1 = nextV;
    }

    kofU = u0;
    kofV = v0;

    return a; 
}

uint64_t modInverse(uint64_t chisloC, uint64_t modulM) {
    int64_t kofU, kofV;
    cout << "  [Лог] Поиск обратного для " << chisloC << " mod " << modulM << ":\n";
    uint64_t nod = extGCD(chisloC, modulM, kofU, kofV);
    
    if (nod != 1) {
        cout << "    Ошибка: обратного числа нет, так как НОД = " << nod << "\n";
        return (uint64_t)(-1);
    }
    
    uint64_t resultat = ((kofU % (int64_t)modulM) + (int64_t)modulM) % modulM;
    cout << "    -> Обратное число найдено: " << resultat << "\n";
    return resultat;
}
