    #include "modulo.h"
    #include <iostream>
    #include <utility>

    using namespace std;

    bool isPrime(uint chisloN) {
        if (chisloN <= 1) return false;
        if (chisloN <= 3) return true;
        if (chisloN % 2 == 0 || chisloN % 3 == 0) return false;
        for (uint indexI = 5; indexI * indexI <= chisloN; indexI += 6) {
            if (chisloN % indexI == 0 || chisloN % (indexI + 2) == 0) return false;
        }
        return true;
    }

    uint gcd(uint a, uint b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    uint powerMod(uint baza, uint stepen, uint modul, bool show_log) {
        uint resultat = 1;
        baza = baza % modul;
        
        if (show_log) {
            cout << "  [Лог] Возведение " << baza << "^" << stepen << " mod " << modul << ":\n";
        }
        
        while (stepen > 0) {
            if (stepen % 2 == 1) {
                resultat = static_cast<uint>((static_cast<__int128>(resultat) * baza) % modul);
                if (show_log) {
                    cout << "    -> Степень нечетная, текущий resultat = " << resultat << "\n";
                }
            }
            baza = static_cast<uint>((static_cast<__int128>(baza) * baza) % modul);
            stepen /= 2;
            if (show_log) {
                cout << "    -> Квадрат основания = " << baza << ", оставшаяся степень = " << stepen << "\n";
            }
        }
        return resultat;
    }

    uint powerModFermat(uint baza, uint stepen, uint modul, bool show_log) {
        if (show_log) {
            cout << "  [Лог Ферма] Проверка условий для " << baza << "^" << stepen << " mod " << modul << ":\n";
        }
        
        if (!isPrime(modul)) {
            if (show_log) {
                cout << "    Ошибка: Модуль " << modul << " не является простым!\n";
            }
            return static_cast<uint>(-1);
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
        
        uint novayaStepen = stepen % (modul - 1);
        if (show_log) {
            cout << "    -> Новая степень: " << stepen << " mod (" << modul << "-1) = " << novayaStepen << "\n";
        }
        
        return powerMod(baza, novayaStepen, modul, show_log);
    }