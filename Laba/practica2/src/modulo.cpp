#include "modulo.h"
#include <iostream>
#include <utility>

using namespace std;

bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

long long powerMod(long long base, long long exp, long long mod, bool show_log) {
    long long res = 1;
    base = base % mod;
    
    if (show_log) {
        cout << "  [Лог] Возведение " << base << "^" << exp << " mod " << mod << ":\n";
    }
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = static_cast<long long>((static_cast<__int128>(res) * base) % mod);
            if (show_log) {
                cout << "    -> Степень нечетная, текущий res = " << res << "\n";
            }
        }
        base = static_cast<long long>((static_cast<__int128>(base) * base) % mod);
        exp /= 2;
        if (show_log) {
            cout << "    -> Квадрат основания = " << base << ", оставшаяся степень = " << exp << "\n";
        }
    }
    return res;
}

long long powerModFermat(long long base, long long exp, long long mod, bool show_log) {
    if (show_log) {
        cout << "  [Лог Ферма] Проверка условий для " << base << "^" << exp << " mod " << mod << ":\n";
    }
    
    if (!isPrime(mod)) {
        if (show_log) {
            cout << "    Ошибка: Модуль " << mod << " не является простым!\n";
        }
        return -1;
    }
    
    if (base % mod == 0) {
        if (show_log) {
            cout << "    -> Основание делится на модуль. Результат: 0\n";
        }
        return 0;
    }
    
    if (show_log) {
        cout << "    -> Условия выполнены. Оптимизируем степень.\n";
    }
    
    long long newExp = exp % (mod - 1);
    if (show_log) {
        cout << "    -> Новая степень: " << exp << " mod (" << mod << "-1) = " << newExp << "\n";
    }
    
    return powerMod(base, newExp, mod, show_log);
}
