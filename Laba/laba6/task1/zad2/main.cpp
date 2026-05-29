#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <windows.h>

using namespace std;

int digitSum(int n) {
    n = abs(n);
    int s = 0;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    srand(static_cast<unsigned>(time(nullptr)));

    int M, N;
    cout << "Введите размеры матрицы M N: ";
    if (!(cin >> M >> N) || M <= 0 || N <= 0) {
        cerr << "Ошибка: размеры должны быть положительными числами.\n";
        return 1;
    }


    vector<vector<int>> matrix(M, vector<int>(N));
    for (auto& row : matrix)
        for (auto& val : row) val = 100 + rand() % 101;

    cout << "\nМАТРИЦА [100..200]:\n";
    for (const auto& row : matrix) {
        for (int val : row) cout << setw(4) << val;
        cout << "\n";
    }

    vector<int> rowDigitSums(M, 0);
    for (int i = 0; i < M; ++i)
        for (int val : matrix[i]) rowDigitSums[i] += digitSum(val);

    cout << "\nСуммы цифр по строкам:\n";
    for (int i = 0; i < M; ++i) cout << "Строка " << i + 1 << ": " << rowDigitSums[i] << "\n";

    int maxRow = max_element(rowDigitSums.begin(), rowDigitSums.end()) - rowDigitSums.begin();
    cout << "\n Строка с НАИБОЛЬШЕЙ суммой цифр: № " << (maxRow + 1) << "\n";

    return 0;
}