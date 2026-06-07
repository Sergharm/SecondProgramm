#include "fractions.h"
#include "euclid.h"
#include <iostream>
using namespace std;

// представление дроби в виде цепной дроби
vector<long long> chain_fraction(long long num, long long den) {
    cout << "\nЦепная дробь" << endl;
    cout << "Раскладываем " << num << "/" << den << endl;
    
    vector<long long> coeffs;
    int step = 1;
    
    while (den != 0) {
        long long q = num / den;
        long long r = num % den;
        coeffs.push_back(q);
        cout << "Шаг " << step << ": " << num << "/" << den << " = " << q;
        if (r != 0) {
            cout << " + " << r << "/" << den;
        }
        cout << " -> q" << step << " = " << q << endl;
        
        num = den;
        den = r;
        step++;
    }
    
    cout << "Коэффициенты цепной дроби: [";
    for (size_t i = 0; i < coeffs.size(); i++) {
        cout << coeffs[i];
        if (i < coeffs.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    return coeffs;
}

// решение диофантова уравнения A*a + B*b = D
void solve_diofant(long long A, long long B, long long D) {
    cout << "\nРешение диофантова уравнения" << endl;
    cout << A << "*a + " << B << "*b = " << D << endl;
    
    // сначала находим gcd через расширенный евклид
    EuclidResult res = ext_gcd(A, B);
    
    // проверяем что решение существует
    if (D % res.gcd != 0) {
        cout << "ОШИБКА: " << D << " не делится на gcd(" << A << ", " << B << ") = " << res.gcd << endl;
        cout << "Решения в целых числах НЕТ!" << endl;
        return;
    }
    
    // частное решение
    long long factor = D / res.gcd;
    long long a0 = res.u * factor;
    long long b0 = res.v * factor;
    
    cout << "\nЧастное решение:" << endl;
    cout << "  a0 = " << res.u << " * " << factor << " = " << a0 << endl;
    cout << "  b0 = " << res.v << " * " << factor << " = " << b0 << endl;
    
    cout << "\nПроверка: " << A << "*(" << a0 << ") + " << B << "*(" << b0 << ") = " 
         << A*a0 + B*b0 << endl;
    
    // общее решение
    long long B_reduced = B / res.gcd;
    long long A_reduced = A / res.gcd;
    
    cout << "\nОбщее решение (через параметр t):" << endl;
    cout << "  a(t) = " << a0 << " + " << B_reduced << "*t" << endl;
    cout << "  b(t) = " << b0 << " - " << A_reduced << "*t" << endl;
    
    cout << "\nПару примеров:" << endl;
    for (int t = -2; t <= 2; t++) {
        long long a_t = a0 + B_reduced * t;
        long long b_t = b0 - A_reduced * t;
        cout << "  t=" << t << ": a=" << a_t << ", b=" << b_t 
             << " -> " << A << "*(" << a_t << ") + " << B << "*(" << b_t << ") = " 
             << A*a_t + B*b_t << endl;
    }
}