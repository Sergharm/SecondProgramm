#include "euclid.h"
#include <iostream>

using namespace std;

uint extGCD(uint a, uint b, int64 &kofU, int64 &kofV) {
    int64 u0 = 1, v0 = 0; 
    int64 u1 = 0, v1 = 1; 
    
    uint shag = 1;
    cout << "[Начало цикла Евклида]\n";

    while (b != 0) {
        uint chastnoeQ = a / b; 
        uint ostatokR = a % b; 

        int64 nextU = u0 - (int64)chastnoeQ * u1;  // Приводим к int64
        int64 nextV = v0 - (int64)chastnoeQ * v1;

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

uint modInverse(uint chisloC, uint modulM) {
    int64 kofU, kofV;
    cout << "  [Лог] Поиск обратного для " << chisloC << " mod " << modulM << ":\n";
    uint nod = extGCD(chisloC, modulM, kofU, kofV);
    
    if (nod != 1) {
        cout << "    Ошибка: обратного числа нет, так как НОД = " << nod << "\n";
        return (uint)(-1);
    }
    
    // Приводим отрицательный коэффициент к положительному остатку
    uint resultat = ((kofU % (int64)modulM) + (int64)modulM) % modulM;
    cout << "    -> Обратное число найдено: " << resultat << "\n";
    return resultat;
}