#include "../include/modulo.h"
#include <iostream>
#include <utility>

using namespace std;

bool isPrime(uint64_t chisloN) {
    if (chisloN <= 1) return false;
    if (chisloN <= 3) return true;
    if (chisloN % 2 == 0 || chisloN % 3 == 0) return false;
    for (uint64_t indexI = 5; indexI * indexI <= chisloN; indexI += 6) {
        if (chisloN % indexI == 0 || chisloN % (indexI + 2) == 0) return false;
    }
    return true;
}

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

uint64_t powerMod(uint64_t baza, uint64_t stepen, uint64_t modul, bool show_log) {
    uint64_t resultat = 1;
    baza = baza % modul;
    
    if (show_log) {
        cout << "Возведение " << baza << "^" << stepen << " mod " << modul << ":\n";
    }
    
    while (stepen > 0) {
        if (stepen % 2 == 1) {
            resultat = static_cast<uint64_t>((static_cast<__int128>(resultat) * baza) % modul);
            if (show_log) {
                cout << "    -> Степень нечетная, текущий resultat = " << resultat << "\n";
            }
        }
        baza = static_cast<uint64_t>((static_cast<__int128>(baza) * baza) % modul);
        stepen /= 2;
        if (show_log) {
            cout << "    -> Квадрат основания = " << baza << ", оставшаяся степень = " << stepen << "\n";
        }
    }
    return resultat;
}

uint64_t powerModFermat(uint64_t baza, uint64_t stepen, uint64_t modul, bool show_log) {
    if (show_log) {
        cout << "Проверка условий для " << baza << "^" << stepen << " mod " << modul << ":\n";
    }
    
    if (!isPrime(modul)) {
        if (show_log) {
            cout << "    Ошибка: Модуль " << modul << " не является простым!\n";
        }
        return static_cast<uint64_t>(-1);
    }
    
    if (baza % modul == 0) {
        if (show_log) {
            cout << "    -> Основание делится на модуль. Результат: 0\n";
        }
        return 0;
    }
    
    if (show_log) {
        cout << "    -> Условия выполнены. Оптимизируем степень.\n";
    }
    
    uint64_t novayaStepen = stepen % (modul - 1);
    if (show_log) {
        cout << "    -> Новая степень: " << stepen << " mod (" << modul << "-1) = " << novayaStepen << "\n";
    }
    
    return powerMod(baza, novayaStepen, modul, show_log);
}
