#include "euclid.h"
#include <iostream>
#include <cstdint>

using namespace std;

// Итеративный расширенный алгоритм Евклида
// Находит НОД(a, b) и коэффициенты Безу u, v такие что a*u + b*v = НОД
uint64_t extGCD(uint64_t a, uint64_t b, int64_t &kofU, int64_t &kofV) {
    // Начальные значения коэффициентов Безу
    int64_t u0 = 1, v0 = 0;   // для a
    int64_t u1 = 0, v1 = 1;   // для b
    
    uint64_t shag = 1;
    cout << "[Начало расширенного алгоритма Евклида]\n";
    cout << "  Ищем u, v такие что " << a << "*u + " << b << "*v = НОД\n";

    while (b != 0) {
        uint64_t chastnoeQ = a / b;   // частное
        uint64_t ostatokR = a % b;    // остаток

        // Обновляем коэффициенты Безу
        int64_t nextU = u0 - static_cast<int64_t>(chastnoeQ) * u1;
        int64_t nextV = v0 - static_cast<int64_t>(chastnoeQ) * v1;

        cout << "  Шаг " << shag++ << ": " << a << " = " << chastnoeQ 
             << " * " << b << " + " << ostatokR << "\n";
        if (ostatokR != 0) {
            cout << "    Коэффициенты: u = " << nextU << ", v = " << nextV << "\n";
        }

        // Сдвигаем значения для следующей итерации
        a = b;
        b = ostatokR;

        u0 = u1; v0 = v1;
        u1 = nextU; v1 = nextV;
    }

    // В a остался НОД, в u0 и v0 — искомые коэффициенты Безу
    kofU = u0;
    kofV = v0;

    cout << "  НОД = " << a << ", u = " << u0 << ", v = " << v0 << "\n";
    return a; 
}

// Нахождение обратного числа c^(-1) mod m
// Использует расширенный алгоритм Евклида: если c*u + m*v = 1, то c^(-1) = u mod m
uint64_t modInverse(uint64_t chisloC, uint64_t modulM) {
    int64_t kofU, kofV;
    cout << "  [Поиск обратного элемента] " << chisloC << "^(-1) mod " << modulM << ":\n";
    uint64_t nod = extGCD(chisloC, modulM, kofU, kofV);
    
    if (nod != 1) {
        cout << "    Ошибка: обратного числа нет, НОД = " << nod << " ≠ 1\n";
        return static_cast<uint64_t>(-1);
    }
    
    // Приводим отрицательный коэффициент kofU к положительному остатку по модулю modulM
    uint64_t resultat = ((kofU % static_cast<int64_t>(modulM)) + static_cast<int64_t>(modulM)) % modulM;
    cout << "    -> Обратное число: " << chisloC << "^(-1) mod " << modulM << " = " << resultat << "\n";
    
    // Проверка через __int128 для защиты от переполнения
    __int128 proverca = (static_cast<__int128>(chisloC) * resultat) % modulM;
    cout << "    Проверка: " << chisloC << " * " << resultat << " mod " << modulM 
         << " = " << static_cast<int64_t>(proverca) << "\n";
    return resultat;
}