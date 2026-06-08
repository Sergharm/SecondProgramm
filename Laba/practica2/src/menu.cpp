#include "menu.h"
#include "modulo.h"
#include "euclid.h"
#include "elgamal.h"
#include "fractions.h"
#include "utils.h"
#include <iostream>
#include <cstdint>

using namespace std;

enum class MenuAction {
    PowerModulo      = 1,
    ExtendedEuclid   = 2,
    ModularInverse   = 3,
    ElGamal          = 4,
    Diophantine      = 5,
    Exit             = 0
};

void showMenu() {
    while (true) {
        cout << "1. Задание 1: Вычисление a^x mod p\n";
        cout << "2. Задание 2: Расширенный алгоритм Евклида\n";
        cout << "3. Задание 3: Модульное обратное число\n";
        cout << "4. Задание 4: Криптосистема Эль-Гамаля\n";
        cout << "5. Задание 6*: Цепные дроби и Диофантово уравнение\n";
        cout << "0. Выход\n";
        cout << "--------------------------------------------\n";
        
        int vybor = static_cast<int>(get_ll("Выберите пункт: ", 0, 5));
        MenuAction action = static_cast<MenuAction>(vybor);

        switch (action) {
            case MenuAction::PowerModulo: {
                cout << "\n[Задание 1: Возведение в степень по модулю]\n";
                uint64_t a = get_ll("Основание a: ", 1);
                uint64_t x = get_ll("Степень x: ", 0);
                uint64_t p = get_ll("Модуль p: ", 2);
                
                cout << "\nМетод 1: Бинарный алгоритм\n";
                uint64_t r1 = powerMod(a, x, p, true);
                cout << "Результат: " << r1 << "\n";
                
                cout << "\nМетод 2: Теорема Ферма\n";
                uint64_t r2 = powerModFermat(a, x, p, true);
                if (r2 != static_cast<uint64_t>(-1)) {
                    cout << "Результат: " << r2 << "\n";
                }
                break;
            }
            case MenuAction::ExtendedEuclid: {
                cout << "\n[Задание 2: Расширенный алгоритм Евклида]\n";
                uint64_t a = get_ll("a: ", 1);
                uint64_t b = get_ll("b: ", 1);
                int64_t kofU, kofV;
                uint64_t nod = extGCD(a, b, kofU, kofV);
                cout << "\nИтог: НОД = " << nod << ", u = " << kofU << ", v = " << kofV << "\n";
                
                __int128 proverca = static_cast<__int128>(a) * kofU + static_cast<__int128>(b) * kofV;
                cout << "Проверка: " << a << "*(" << kofU << ") + " << b << "*(" << kofV 
                     << ") = " << static_cast<int64_t>(proverca) << "\n";
                break;
            }
            case MenuAction::ModularInverse: {
                cout << "\n[Задание 3: Обратный элемент по модулю]\n";
                uint64_t c = get_ll("c: ", 1);
                uint64_t m = get_ll("m: ", 2);
                modInverse(c, m);
                break;
            }
            case MenuAction::ElGamal: {
                runElGamalMenu();
                break;
            }
            case MenuAction::Diophantine: {
                solveDiophantineVariant3();
                break;
            }
            case MenuAction::Exit: {
                cout << "Выход из программы.\n";
                return;
            }
            default: {
                cout << "Неверный выбор. Попробуйте снова.\n";
                break;
            }
        }
    }
}