#include "func.h"

int** allocMatrix(int n, int m) {
    int** mat = new int*[n];
    for (int i = 0; i < n; ++i) mat[i] = new int[m];
    return mat;
}

void freeMatrix(int** mat, int n) {
    for (int i = 0; i < n; ++i) delete[] mat[i];
    delete[] mat;
}

void printBoard(int** board, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int dist(Point p1, Point p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

unsigned int findPairCost(const vector<Point>& mism, int idx) {
    if (idx >= mism.size()) return 0;

    unsigned int minCost = UINT_MAX;

    for (size_t i = idx + 1; i < mism.size(); ++i) {
        vector<Point> nextMism = mism;
        nextMism.erase(nextMism.begin() + i);
        nextMism.erase(nextMism.begin() + idx);

        unsigned int currentDist = dist(mism[idx], mism[i]);
        unsigned int restCost = findPairCost(nextMism, idx);

        if (restCost != UINT_MAX) {
            unsigned int total = currentDist + restCost;
            if (total < minCost) minCost = total;
        }
    }
    return minCost;
}

unsigned int calcMinMovesHalf(int** board, int n, int m, int topVal, vector<Point>& mismatches) {
    mismatches.clear();
    int bottomVal = 1 - topVal;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int expected = (i < n / 2) ? topVal : bottomVal;
            if (board[i][j] != expected) {
                mismatches.push_back({i, j});
            }
        }
    }

    if (mismatches.size() % 2 != 0) return UINT_MAX;
    return findPairCost(mismatches, 0);
}

// Новая симуляция: идем по порядку и исправляем ошибки
void simulateStepsHalf(int** board, int n, int m, int topVal) {
    int stepCount = 1;
    
    // Проходим по каждой клетке матрицы
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int expected;
            if (i < n / 2) expected = topVal;
            else expected = 1 - topVal;

            // Если клетка не совпадает с ожидаемой, совершаем ход
            if (board[i][j] != expected) {
                int ni = i; 
                int nj = j;
                
                // Стратегия из примера: переворачиваем с соседом справа,
                // если нельзя (конец строки) - с соседом снизу
                if (j + 1 < m) {
                    nj = j + 1; 
                } else {
                    ni = i + 1; 
                }

                // Переворот пары
                board[i][j] = 1 - board[i][j];
                board[ni][nj] = 1 - board[ni][nj];

                cout << "Шаг " << stepCount << ":" << endl;
                printBoard(board, n, m);
                cout << endl;

                stepCount++;
            }
        }
    }
}