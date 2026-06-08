#include "fractions.h"
#include "euclid.h"
#include <iostream>
#include <vector>

using namespace std;

void solveDiophantineVariant3() {
    cout << "\nДиофантово уравнение\n";
    cout << "Уравнение: 1256a + 847b = 119\n\n";
    
    long long A = 1256, B = 847, D = 119;
    
    long long u_gcd, v_gcd;
    long long g = extGCD(A, B, u_gcd, v_gcd);
    cout << "НОД(" << A << ", " << B << ") = " << g << "\n";
    
    if (D % g != 0) {
        cout << "Решений в целых числах нет, так как 119 не делится на НОД.\n";
        return;
    }
    
    // Наглядное разложение в цепную дробь
    cout << "\n[Разложение в цепную дробь]:\n";
    vector<long long> q;
    long long tempA = A, tempB = B;
    while (tempB != 0) {
        q.push_back(tempA / tempB);
        cout << "    " << tempA << " / " << tempB << " = " << (tempA / tempB) << " (ост. " << (tempA % tempB) << ")\n";
        long long t = tempA % tempB;
        tempA = tempB;
        tempB = t;
    }
    
    cout << "Коэффициенты цепной дроби:";
    for (size_t i = 0; i < q.size(); ++i) {
        cout << q[i] << (i == q.size() - 1 ? "" : ", ");
    }

    // Нахождение ответов
    long long factor = D / g;
    long long a0 = u_gcd * factor;
    long long b0 = v_gcd * factor;
    
    cout << "\n  [Ответ]:\n";
    cout << "  Частное решение: a0 = " << a0 << ", b0 = " << b0 << "\n";
    cout << "  Общее решение через параметр t:\n";
    cout << "    a = " << a0 << " + " << (B / g) << " * t\n";
    cout << "    b = " << b0 << " - " << (A / g) << " * t\n";
}
