#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <windows.h>

using namespace std;

void printMatrix(const vector<vector<int>>& mat, const string& title) {
    cout << "\n" << title << "\n";
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            cout << setw(4) << mat[i][j];
        }
        cout << "\n";
    }
    cout << string(60, '-') << "\n";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    srand(static_cast<unsigned>(time(nullptr)));

    int M, N;
    cout << "Введите количество строк M (>0): ";
    cin >> M;
    cout << "Введите количество столбцов N (>0): ";
    cin >> N;

    // Валидация ввода
    if (M <= 0 || N <= 0) {
        cerr << "Ошибка: размеры матрицы должны быть положительными числами.\n";
        return 1;
    }

    vector<vector<int>> matrix(M, vector<int>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = 10 + rand() % 41;
        }
    }

    printMatrix(matrix, "ИСХОДНАЯ МАТРИЦА:");

    vector<int> rowDiffs(M); 

    for (int i = 0; i < M; ++i) {
        int minIdx = 0, maxIdx = 0;

        for (int j = 1; j < N; ++j) {
            if (matrix[i][j] < matrix[i][minIdx]) minIdx = j; 
            if (matrix[i][j] > matrix[i][maxIdx]) maxIdx = j; 
        }

        int minVal = matrix[i][minIdx];
        int maxVal = matrix[i][maxIdx];

        swap(matrix[i][minIdx], matrix[i][maxIdx]);

        rowDiffs[i] = maxVal - minVal;
    }

    printMatrix(matrix, "МАТРИЦА ПОСЛЕ МОДИФИКАЦИИ (swap min/max в каждой строке):");

    cout << "Разности (max - min) для каждой строки:\n";
    for (int i = 0; i < M; ++i) {
        cout << "Строка " << (i + 1) << ": " << rowDiffs[i] << "\n";
    }

    int maxDiffRow = 0;
    for (int i = 1; i < M; ++i) {
        if (rowDiffs[i] > rowDiffs[maxDiffRow]) {
            maxDiffRow = i;
        }
    }

    cout << "Строка с максимальной разностью: № " << (maxDiffRow + 1) << " (разность = " << rowDiffs[maxDiffRow] << ")\n";

    return 0;
}