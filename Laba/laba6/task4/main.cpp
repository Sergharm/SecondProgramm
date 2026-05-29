#include "func.h"

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");

    cout << "Задание 4, Вариант 3 (Паттерн 'Половина')" << endl << endl;

    int n, m;
    cout << "Введите размеры доски N и M: ";
    cin >> n >> m;

    int** board = allocMatrix(n, m);

    cout << "Введите состояние доски (0 или 1):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> board[i][j];
        }
    }

    cout << "\nИсходная доска:" << endl;
    printBoard(board, n, m);

    // Проверяем два возможных целевых состояния:
    vector<Point> mismatchesA;
    unsigned int costA = calcMinMovesHalf(board, n, m, 0, mismatchesA); // Верх=0

    vector<Point> mismatchesB;
    unsigned int costB = calcMinMovesHalf(board, n, m, 1, mismatchesB); // Верх=1

    unsigned int minMoves = UINT_MAX;
    int bestTopVal = -1;

    if (costA != UINT_MAX && (costA < minMoves || costB == UINT_MAX)) {
        minMoves = costA;
        bestTopVal = 0;
    }
    
    if (costB != UINT_MAX && costB < minMoves) {
        minMoves = costB;
        bestTopVal = 1;
    }

    if (minMoves == UINT_MAX) {
        cout << "\nНевозможно достичь паттерна 'Половина' данными ходами." << endl;
    } else {
        cout << "Минимальное число ходов: " << minMoves << endl;
        cout << "Целевой паттерн: Верх=" << bestTopVal << ", Низ=" << (1 - bestTopVal) << endl << endl;
        
        cout << "Симуляция решения:" << endl;
        simulateStepsHalf(board, n, m, bestTopVal);
    }

    freeMatrix(board, n);
    return 0;
}