#include "modulo.h"
#include <iostream>
using namespace std;

// проверка на простоту перебором (ну а чё, для лабы хватит)
bool is_prime(long long chislo) {
    if (chislo < 2) return false;
    if (chislo == 2 || chislo == 3) return true;
    if (chislo % 2 == 0) return false;
    for (long long i = 3; i * i <= chislo; i += 2) {
        if (chislo % i == 0) return false;
    }
    return true;
}

// безопасное умножение через __int128
// без этого переполнение будет при больших числах, препод говорил
long long safe_mul(long long a, long long b, long long mod) {
    __int128 aa = a;
    __int128 bb = b;
    __int128 res = (aa * bb) % mod;
    return (long long)res;
}

// бинарное возведение в степень (быстрое, за логарифм)
long long binary_pow(long long a, long long x, long long m) {
    cout << "\n=== Бинарное возведение в степень ===" << endl;
    cout << "Считаем " << a << "^" << x << " mod " << m << endl;
    
    long long result = 1 % m;
    a = a % m;
    int step = 1;
    
    while (x > 0) {
        cout << "Шаг " << step << ": x = " << x;
        
        // если степень нечётная - умножаем результат на основание
        if (x % 2 == 1) {
            result = safe_mul(result, a, m);
            cout << " (нечётная) -> result = " << result;
        }
        
        // основание в квадрат
        a = safe_mul(a, a, m);
        x = x / 2;
        cout << ", новое a = " << a << endl;
        step++;
    }
    
    cout << "Итого результат: " << result << endl;
    return result;
}

// возведение в степень через малую теорему ферма
// работает только если p простое и a не делится на p
long long fermat_pow(long long a, long long x, long long p) {
    cout << "\n=== Возведение в степень по теореме Ферма ===" << endl;
    cout << "Считаем " << a << "^" << x << " mod " << p << endl;
    
    // проверка условий теоремы
    if (!is_prime(p)) {
        cout << "ОШИБКА: p = " << p << " не простое число!" << endl;
        cout << "Теорема Ферма тут не работает, извиняйте." << endl;
        return -1;
    }
    cout << "Условие 1 OK: p = " << p << " - простое число" << endl;
    
    if (a % p == 0) {
        cout << "Внимание: a делится на p, результат будет 0" << endl;
        return 0;
    }
    cout << "Условие 2 OK: a = " << a << " и p = " << p << " взаимно просты" << endl;
    
    // по теореме ферма a^(p-1) = 1 mod p
    // значит a^x = a^(x mod (p-1)) mod p
    long long new_x = x % (p - 1);
    cout << "Сокращаем степень по теореме Ферма:" << endl;
    cout << "  x mod (p-1) = " << x << " mod " << (p - 1) << " = " << new_x << endl;
    cout << "Теперь считаем " << a << "^" << new_x << " mod " << p << endl;
    
    // дальше считаем бинарным методом
    return binary_pow(a, new_x, p);
}