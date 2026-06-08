#include <iostream>
#include <cmath>
#include <cstdint>

using namespace std;

uint64_t nod(uint64_t a, uint64_t b) { // НОД
    while (b != 0) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    uint64_t a;
    uint64_t b;
    cin >> a >> b;

    if (b <= 1) { // если ряд расходится
        cout << "infinity" << endl;
        return 0;
    }

    double sum = 0; // накопленная сумма ряда
    double term = 0; // текущее слагаемое

    for (uint64_t n = 1; n <= 20000; n++) { // вычисляем сумму ряда

        double numerator = 1;
        for (uint64_t i = 0; i < a; i++) {  // Вычисляем n^a
            numerator *= n;
        }

        double denominator = 1;
        for (uint64_t i = 0; i < n; i++) {  // Вычисляем b^n
            denominator *= b;
        }

        term = numerator / denominator;
        sum += term;

        // Если слагаемое стало очень маленьким, останавливаемся
        if (term < 1e-18) {
            break;
        }
    }

    uint64_t bestNum = 1;
    uint64_t bestDen = 1;
    double bestDiff = 1e20;

    // Перебираем знаменатели, чтобы найти лучшую дробь
    for (uint64_t den = 1; den <= 2000000; den++) {
        // Находим ближайший числитель
        uint64_t num = (uint64_t)(sum * den + 0.5);

        // Проверяем насколько близко
        double diff = fabs((double)num / den - sum);

        if (diff < bestDiff) {
            bestDiff = diff;
            bestNum = num;
            bestDen = den;
            // Если нашли очень точное совпадение - можно остановиться
            if (diff < 1e-15) {
                break;
            }
        }
    }

    // Сокращаем дробь
    uint64_t common = nod(bestNum, bestDen);
    cout << bestNum / common << "/" << bestDen / common << endl;

    return 0;
}