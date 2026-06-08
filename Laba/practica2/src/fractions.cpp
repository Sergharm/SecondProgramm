#include "fractions.h"
#include "euclid.h"
#include <iostream>
#include <vector>

using namespace std;

typedef unsigned long long uint;
typedef long long int64;

void solveDiophantineVariant3() {
    cout << "\nДиофантово уравнение\n";
    cout << "Уравнение: 1256a + 847b = 119\n\n";
    
    uint A = 1256, B = 847, D = 119;
    
    int64 u_gcd, v_gcd;
    uint nod = extGCD(A, B, u_gcd, v_gcd);
    cout << "НОД(" << A << ", " << B << ") = " << nod << "\n";
    
    if (D % nod != 0) {
        cout << "Решений в целых числах нет.\n";
        return;
    }
    
    // Наглядное разложение в цепную дробь
    cout << "\n[Разложение в цепную дробь]:\n";
    vector<uint> kof;
    uint vremennoeA = A, vremennoeB = B;
    while (vremennoeB != 0) {
        kof.push_back(vremennoeA / vremennoeB);
        cout << "    " << vremennoeA << " / " << vremennoeB 
             << " = " << (vremennoeA / vremennoeB) 
             << " (ост. " << (vremennoeA % vremennoeB) << ")\n";
        uint t = vremennoeA % vremennoeB;
        vremennoeA = vremennoeB;
        vremennoeB = t;
    }
    
    cout << "Коэффициенты цепной дроби:";
    for (size_t i = 0; i < kof.size(); ++i) {
        cout << kof[i] << (i == kof.size() - 1 ? "" : ", ");
    }

    // Нахождение ответов
    int64 mnozhitel = D / nod;
    int64 answerA0 = u_gcd * mnozhitel;
    int64 answerB0 = v_gcd * mnozhitel;
    
    cout << "\n  [Ответ]:\n";
    cout << "  Частное решение: a0 = " << answerA0 
         << ", b0 = " << answerB0 << "\n";
    cout << "  Общее решение через параметр t:\n";
    cout << "    a = " << answerA0 << " + " << (B / nod) << " * t\n";
    cout << "    b = " << answerB0 << " - " << (A / nod) << " * t\n";
}