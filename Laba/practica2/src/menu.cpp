#include "menu.h"
#include "modulo.h"
#include "euclid.h"
#include "elgamal.h"
#include "fractions.h"
#include <iostream>
#include <limits>

using namespace std;

typedef long long int64;  // Добавляем в начало файла

void showMenu() {
    int vybor = -1;
    while (vybor != 0) {
        cout << "1. Задание 1: Вычисление a^x mod p\n";
        cout << "2. Задание 2: Расширенный алгоритм Евклида\n";
        cout << "3. Задание 3: Модульное обратное число\n";
        cout << "4. Задание 4: Криптосистема Эль-Гамаля\n";
        cout << "5. Задание 6*: Цепные дроби и Диофантово уравнение\n";
        cout << "0. Выход\n";
        
        vybor = static_cast<int>(get_ll("Выберите пункт: ", 0, 5));

        if (vybor == 1) {
            uint a = get_ll("Введите основание a: ", 1);
            uint x = get_ll("Введите степень x: ", 0);
            uint p = get_ll("Введите модуль p: ", 2);
            
            cout << "\n Метод 1: Бинарный алгоритм\n";
            uint r1 = powerMod(a, x, p);
            cout << "Результат: " << r1 << "\n";
            
            cout << "\nМетод 2: Теорема Ферма\n";
            uint r2 = powerModFermat(a, x, p);
            if (r2 != static_cast<uint>(-1)) cout << "Результат: " << r2 << "\n";
        } 
        else if (vybor == 2) {
            uint a = get_ll("Введите a: ", 1);
            uint b = get_ll("Введите b: ", 1);
            int64 kofU, kofV;  // ✅ ИСПРАВЛЕНО
            uint nod = extGCD(a, b, kofU, kofV);
            cout << "\nНОД = " << nod << ", u = " << kofU << ", v = " << kofV << "\n";
        } 
        else if (vybor == 3) {
            uint c = get_ll("Введите c: ", 1);
            uint m = get_ll("Введите m: ", 2);
            modInverse(c, m);
        } 
        else if (vybor == 4) {
            runElGamalMenu();
        }
        else if (vybor == 5) {
            solveDiophantineVariant3();
        }
    }
}