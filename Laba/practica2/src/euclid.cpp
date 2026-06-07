#include "euclid.h"
#include <iostream>
using namespace std;

// расширенный алгоритм евклида
// ищем u, v такие что a*u + b*v = gcd(a,b)
EuclidResult ext_gcd(long long a, long long b) {
    cout << "\nРасширенный алгоритм Евклида" << endl;
    cout << "Ищем u, v для: " << a << "*u + " << b << "*v = gcd(" << a << ", " << b << ")" << endl;
    
    long long old_r = a, r = b;
    long long old_u = 1, u = 0;
    long long old_v = 0, v = 1;
    int iter = 1;
    
    cout << "Начальные значения:" << endl;
    cout << "  r0=" << old_r << ", r1=" << r << endl;
    cout << "  u0=" << old_u << ", u1=" << u << endl;
    cout << "  v0=" << old_v << ", v1=" << v << endl;
    
    while (r != 0) {
        long long q = old_r / r;
        cout << "Итерация " << iter << ":" << endl;
        cout << "  q = " << old_r << " / " << r << " = " << q << endl;
        
        // сохраняем старое r
        long long tmp_r = r;
        r = old_r - q * r;
        old_r = tmp_r;
        
        // обновляем u
        long long tmp_u = u;
        u = old_u - q * u;
        old_u = tmp_u;
        
        // обновляем v
        long long tmp_v = v;
        v = old_v - q * v;
        old_v = tmp_v;
        
        cout << "  Новые значения: r=" << r << ", u=" << u << ", v=" << v << endl;
        iter++;
    }
    
    cout << "Результат:" << endl;
    cout << "  gcd = " << old_r << endl;
    cout << "  u = " << old_u << endl;
    cout << "  v = " << old_v << endl;
    cout << "Проверка: " << a << "*(" << old_u << ") + " << b << "*(" << old_v << ") = " 
         << a*old_u + b*old_v << endl;
    
    EuclidResult res;
    res.u = old_u;
    res.v = old_v;
    res.gcd = old_r;
    return res;
}

// поиск обратного элемента по модулю
// c^(-1) mod m = d, то есть c*d = 1 mod m
long long mod_inverse(long long c, long long m) {
    cout << "\n=== Поиск обратного элемента ===" << endl;
    cout << "Ищем " << c << "^(-1) mod " << m << endl;
    
    EuclidResult res = ext_gcd(c, m);
    
    if (res.gcd != 1) {
        cout << "ОШИБКА: gcd(" << c << ", " << m << ") = " << res.gcd << " != 1" << endl;
        cout << "Обратного элемента НЕ СУЩЕСТВУЕТ!" << endl;
        return -1;
    }
    
    // приводим к положительному числу в диапазоне [0, m-1]
    long long d = (res.u % m + m) % m;
    cout << "Обратный элемент: " << c << "^(-1) mod " << m << " = " << d << endl;
    cout << "Проверка: " << c << " * " << d << " mod " << m << " = " 
         << (c * d) % m << endl;
    
    return d;
}