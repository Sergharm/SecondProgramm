#include "menu.h"
#include "utils.h"
#include "modulo.h"
#include "euclid.h"
#include "elgamal.h"
#include "fractions.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

void show_menu() {
    srand(time(0));
    int choice;
    
    while (true) {
        cout << "\n============================================" << endl;
        cout << "  Практическая работа №2 (Вариант 3)" << endl;
        cout << "============================================" << endl;
        cout << "1. Возведение в степень (Ферма)\n2. Возведение в степень (бинарное)\n";
        cout << "3. Расширенный Евклид (u, v)\n4. Обратный элемент\n";
        cout << "5. Эль-Гамаль: ключи\n6. Эль-Гамаль: шифрование\n";
        cout << "7. Эль-Гамаль: дешифрование\n8. Цепная дробь\n";
        cout << "9. Диофантово уравнение (1256a+847b=119)\n0. Выход\n";
        cout << "--------------------------------------------" << endl;
        cout << "Выберите пункт: ";
        
        if (!(cin >> choice)) {
            cout << "Ошибка: введите число от 0 до 9!\n";
            clear_input();
            continue;
        }
        clear_input();
        
        if (choice == 0) { cout << "Пока!\n"; break; }
        if (choice < 1 || choice > 9) {
            cout << "Ошибка: нет такого пункта.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                long long a = get_ll("a = ", -1000000, 10000000);
                long long x = get_ll("x = ", 0, 100000000);
                long long p = get_ll("p (модуль) = ", 2, 10000000);
                
                if (!is_prime(p)) {
                    cout << "Ошибка: p = " << p << " не простое. Теорема Ферма неприменима.\n";
                } else if (a % p == 0) {
                    cout << "Внимание: a делится на p, результат будет 0.\n";
                    cout << "Результат: 0\n";
                } else {
                    long long res = fermat_pow(a, x, p);
                    if (res != -1) cout << "Результат: " << res << "\n";
                }
                break;
            }
            case 2: {
                long long a = get_ll("a = ", -1000000, 10000000);
                long long x = get_ll("x = ", 0, 100000000);
                long long m = get_ll("m = ", 2, 10000000);
                cout << "Результат: " << binary_pow(a, x, m) << "\n";
                break;
            }
            case 3: {
                long long a = get_ll("a = ", 1, 10000000);
                long long b = get_ll("b = ", 1, 10000000);
                ext_gcd(a, b);
                break;
            }
            case 4: {
                long long c = get_ll("c = ", 1, 10000000);
                long long m = get_ll("m = ", 2, 10000000);
                long long d = mod_inverse(c, m);
                if (d != -1) cout << "Обратный элемент: " << d << "\n";
                break;
            }
            case 5: {
                long long p = get_ll("Простое p = ", 2, 10000000);
                if (!is_prime(p)) { cout << "Ошибка: p должно быть простым!\n"; break; }
                long long g = get_ll("Образующий g = ", 2, p-1);
                Keys k = gen_keys(p, g);
                if (k.p != 0) {
                    cout << "\nОткрытый: (p=" << k.p << ", g=" << k.g << ", y=" << k.y << ")\n";
                    cout << "Закрытый x = " << k.x << " (ЗАПИШИТЕ!)\n";
                }
                break;
            }
            case 6: {
                string in = get_existing_file("Входной файл: ");
                string out = get_new_file("Выходной файл: ");
                long long p = get_ll("Простое p = ", 2, 10000000);
                if (!is_prime(p)) { cout << "Ошибка: p не простое!\n"; break; }
                long long g = get_ll("Образующий g = ", 2, p-1);
                Keys k = gen_keys(p, g);
                if (k.p != 0) {
                    encrypt_file(in, out, k);
                    cout << "\nЗакрытый ключ x = " << k.x << " нужен для дешифрования!\n";
                }
                break;
            }
            case 7: {
                string in = get_existing_file("Зашифрованный файл: ");
                string out = get_new_file("Расшифрованный файл: ");
                long long x = get_ll("Закрытый ключ x = ", 1, 10000000);
                long long p = get_ll("Простое p = ", 2, 10000000);
                decrypt_file(in, out, x, p);
                break;
            }
            case 8: {
                long long num = get_ll("Числитель = ", -1000000, 10000000);
                long long den = get_ll("Знаменатель = ", 1, 10000000);
                chain_fraction(num, den);
                break;
            }
            case 9: {
                cout << "Решение уравнения 1256a + 847b = 119\n";
                solve_diofant(1256, 847, 119);
                break;
            }
        }
    }
}