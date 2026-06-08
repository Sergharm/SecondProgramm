<<<<<<< HEAD
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
=======
#include "modulo.h"
#include <iostream>
#include <utility>
#include <cstdint>

using namespace std;

bool isPrime(uint64 chisloN) {
    if (chisloN <= 1) return false;
    if (chisloN <= 3) return true;
    if (chisloN % 2 == 0 || chisloN % 3 == 0) return false;
    for (uint64 i = 5; i * i <= chisloN; i += 6) {
        if (chisloN % i == 0 || chisloN % (i + 2) == 0) return false;
>>>>>>> d6e8ed2 (Изменения во второй практике)
    }
    return true;
}

<<<<<<< HEAD
uint64_t gcd(uint64_t a, uint64_t b) {
=======
uint64 gcd(uint64 a, uint64 b) {
>>>>>>> d6e8ed2 (Изменения во второй практике)
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

<<<<<<< HEAD
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
=======
uint64 powerMod(uint64 baza, uint64 stepen, uint64 modul, bool show_log) {
    uint64 resultat = 1;
    baza = baza % modul;
    
    if (show_log) {
        cout << "  [Лог] Возведение " << baza << "^" << stepen << " mod " << modul << ":\n";
    }
    
    uint64 shag = 1;
    while (stepen > 0) {
        if (stepen % 2 == 1) {
            resultat = static_cast<uint64>((static_cast<__int128>(resultat) * baza) % modul);
            if (show_log) {
                cout << "    Шаг " << shag << ": степень нечётная, resultat = " << resultat << "\n";
            }
        }
        baza = static_cast<uint64>((static_cast<__int128>(baza) * baza) % modul);
        stepen /= 2;
        if (show_log && stepen > 0) {
            cout << "    Шаг " << shag << ": квадрат основания = " << baza << ", остаток степени = " << stepen << "\n";
        }
        shag++;
    }
    
    if (show_log) {
        cout << "  Итого: " << resultat << "\n";
>>>>>>> d6e8ed2 (Изменения во второй практике)
    }
    return resultat;
}

<<<<<<< HEAD
uint64_t powerModFermat(uint64_t baza, uint64_t stepen, uint64_t modul, bool show_log) {
    if (show_log) {
        cout << "Проверка условий для " << baza << "^" << stepen << " mod " << modul << ":\n";
=======
uint64 powerModFermat(uint64 baza, uint64 stepen, uint64 modul, bool show_log) {
    if (show_log) {
        cout << "  [Лог Ферма] Проверка условий для " << baza << "^" << stepen << " mod " << modul << ":\n";
>>>>>>> d6e8ed2 (Изменения во второй практике)
    }
    
    if (!isPrime(modul)) {
        if (show_log) {
            cout << "    Ошибка: Модуль " << modul << " не является простым!\n";
        }
<<<<<<< HEAD
        return static_cast<uint64_t>(-1);
=======
        return static_cast<uint64>(-1);
    }
    if (show_log) {
        cout << "    Условие 1 OK: p = " << modul << " — простое\n";
>>>>>>> d6e8ed2 (Изменения во второй практике)
    }
    
    if (baza % modul == 0) {
        if (show_log) {
<<<<<<< HEAD
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
=======
            cout << "    Основание делится на модуль. Результат: 0\n";
        }
        return 0;
    }
    if (show_log) {
        cout << "    Условие 2 OK: gcd(a, p) = 1\n";
    }
    
    uint64 novayaStepen = stepen % (modul - 1);
    if (show_log) {
        cout << "    Сокращаем степень: " << stepen << " mod (" << modul << "-1) = " << novayaStepen << "\n";
    }
    
    return powerMod(baza, novayaStepen, modul, show_log);
}
>>>>>>> d6e8ed2 (Изменения во второй практике)
