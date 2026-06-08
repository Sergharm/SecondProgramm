#include "menu.h"
#include "modulo.h"
#include "euclid.h"
#include "elgamal.h"
#include "fractions.h"
#include <iostream>
#include <limits>

using namespace std;

void showMenu() {
    int choice = -1;
    while (choice != 0) {
        cout << "1. Задание 1: Вычисление a^x mod p\n";
        cout << "2. Задание 2: Расширенный алгоритм Евклида\n";
        cout << "3. Задание 3: Модульное обратное число\n";
        cout << "4. Задание 4: Криптосистема Эль-Гамаля\n";
        cout << "5. Задание 6*: Цепные дроби и Диофантово уравнение\n";
        cout << "0. Выход\n";
        
        choice = static_cast<int>(get_ll("Выберите пункт: ", 0, 5));

        if (choice == 1) {
            long long a = get_ll("Введите основание a: ", 1);
            long long x = get_ll("Введите степень x: ", 0);
            long long p = get_ll("Введите модуль p: ", 2);
            
            cout << "\n Метод 1: Бинарный алгоритм\n";
            long long r1 = powerMod(a, x, p);
            cout << "Результат: " << r1 << "\n";
            
            cout << "\nМетод 2: Теорема Ферма\n";
            long long r2 = powerModFermat(a, x, p);
            if (r2 != -1) cout << "Результат: " << r2 << "\n";
        } 
        else if (choice == 2) {
            long long a = get_ll("Введите a: ", 1);
            long long b = get_ll("Введите b: ", 1);
            long long u, v;
            long long g = extGCD(a, b, u, v);
            cout << "\nНОД = " << g << ", u = " << u << ", v = " << v << "\n";
        } 
        else if (choice == 3) {
            long long c = get_ll("Введите c: ", 1);
            long long m = get_ll("Введите m: ", 2);
            modInverse(c, m);
        } 
        else if (choice == 4) {
            runElGamalMenu(); // Вызов нового интерактивного подменю
        }

        else if (choice == 5) {
            solveDiophantineVariant3();
        }
    }
}
