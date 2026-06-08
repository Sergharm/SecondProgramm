#include "fractions.h"
#include "euclid.h"
#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Решение диофантова уравнения 1256a + 847b = 119 (Вариант 3)
// Использует расширенный алгоритм Евклида и цепные дроби
void solveDiophantineVariant3() {
    cout << "\n=== Диофантово уравнение ===\n";
    cout << "Уравнение: 1256a + 847b = 119\n\n";
    
    uint64_t A = 1256, B = 847, D = 119;
    
    // Шаг 1: Находим НОД(A, B) и коэффициенты Безу через расширенный алгоритм Евклида
    int64_t u_gcd, v_gcd;
    uint64_t nod = extGCD(A, B, u_gcd, v_gcd);
    cout << "НОД(" << A << ", " << B << ") = " << nod << "\n";
    
    // Шаг 2: Проверяем существование решения
    // Уравнение A*a + B*b = D имеет решение тогда и только тогда, когда НОД(A,B) делит D
    if (D % nod != 0) {
        cout << "Решений в целых числах нет (D не делится на НОД).\n";
        return;
    }
    
    // Шаг 3: Разложение дроби A/B в цепную дробь
    // Коэффициенты цепной дроби — это частные на каждом шаге алгоритма Евклида
    cout << "\n[Разложение " << A << "/" << B << " в цепную дробь]:\n";
    vector<uint64_t> kof;
    uint64_t vremennoeA = A, vremennoeB = B;
    int shag = 1;
    while (vremennoeB != 0) {
        uint64_t q = vremennoeA / vremennoeB;
        uint64_t r = vremennoeA % vremennoeB;
        kof.push_back(q);
        cout << "  Шаг " << shag++ << ": " << vremennoeA << " / " << vremennoeB 
             << " = " << q << " (остаток " << r << ")\n";
        vremennoeA = vremennoeB;
        vremennoeB = r;
    }
    
    cout << "Коэффициенты цепной дроби: [";
    for (size_t i = 0; i < kof.size(); ++i) {
        cout << kof[i] << (i == kof.size() - 1 ? "" : ", ");
    }
    cout << "]\n";

    // Шаг 4: Находим частное решение
    // Из расширенного алгоритма: A*u_gcd + B*v_gcd = НОД
    // Умножаем обе части на D/НОД: A*(u_gcd*D/НОД) + B*(v_gcd*D/НОД) = D
    int64_t mnozhitel = D / nod;
    int64_t answerA0 = u_gcd * mnozhitel;
    int64_t answerB0 = v_gcd * mnozhitel;
    
    cout << "\n[Решение]:\n";
    cout << "  Частное решение:\n";
    cout << "    a0 = " << u_gcd << " * " << mnozhitel << " = " << answerA0 << "\n";
    cout << "    b0 = " << v_gcd << " * " << mnozhitel << " = " << answerB0 << "\n";
    
    // Проверка частного решения через __int128
    __int128 proverka = static_cast<__int128>(A) * answerA0 + static_cast<__int128>(B) * answerB0;
    cout << "  Проверка: " << A << "*(" << answerA0 << ") + " << B << "*(" << answerB0 
         << ") = " << static_cast<int64_t>(proverka) << "\n";
    
    // Шаг 5: Общее решение
    // a(t) = a0 + (B/НОД)*t
    // b(t) = b0 - (A/НОД)*t
    cout << "\n  Общее решение (параметр t — любое целое):\n";
    cout << "    a(t) = " << answerA0 << " + " << (B / nod) << " * t\n";
    cout << "    b(t) = " << answerB0 << " - " << (A / nod) << " * t\n";
    
    // Примеры решений при разных значениях t
    cout << "\n  Примеры при разных t:\n";
    for (int t = -2; t <= 2; ++t) {
        int64_t a_t = answerA0 + (B / nod) * t;
        int64_t b_t = answerB0 - (A / nod) * t;
        __int128 res = static_cast<__int128>(A) * a_t + static_cast<__int128>(B) * b_t;
        cout << "    t = " << t << ": a = " << a_t << ", b = " << b_t 
             << " -> " << A << "*a + " << B << "*b = " << static_cast<int64_t>(res) << "\n";
    }
}