#include "euclid.h"
#include <iostream>

using namespace std;

// Итеративный расширенный алгоритм Евклида с выводом u, v, q
long long extGCD(long long a, long long b, long long &u, long long &v) {
    long long u0 = 1, v0 = 0; 
    long long u1 = 0, v1 = 1; 
    
    long long step = 1;
    cout << "[Начало цикла Евклида]\n";

    while (b != 0) {
        long long q = a / b; 
        long long r = a % b; 

        long long next_u = u0 - q * u1;
        long long next_v = v0 - q * v1;

        // Теперь мы выводим коэффициенты, которые относятся именно к полученному остатку r
        cout << "    Шаг " << step++ << ": " << a << " / " << b << " = " << q << " (остаток " << r << ")\n";
        if (r != 0) {
            cout << "          Коэффициенты для остатка " << r << ": q = " << q 
                 << ", u = " << next_u << ", v = " << next_v << "\n";
            cout << "          Проверка шага: " << next_u << "*7 + " << next_v << "*17 = " << r << "\n";
        } else {
            cout << "          Остаток 0, деление завершено. НОД найден.\n";
        }

        a = b;
        b = r;

        u0 = u1; v0 = v1;
        u1 = next_u; v1 = next_v;
    }

    u = u0;
    v = v0;

    return a; 
}


// Нахождение обратного числа c^-1 mod m
long long modInverse(long long c, long long m) {
    long long u, v;
    cout << "  [Лог] Поиск обратного для " << c << " mod " << m << ":\n";
    long long g = extGCD(c, m, u, v);
    
    if (g != 1) {
        cout << "    Ошибка: обратного числа нет, так как НОД = " << g << " (не равен 1)\n";
        return -1;
    }
    
    // Переводим отрицательный коэффициент u в положительный остаток по модулю m
    long long res = (u % m + m) % m;
    cout << "    -> Обратное число найдено: " << res << "\n";
    return res;
}
