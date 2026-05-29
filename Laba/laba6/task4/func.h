#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <algorithm>
#include <locale>
#include <windows.h>

using namespace std;

struct Point {
    int r;
    int c;
};

int** allocMatrix(int n, int m);
void freeMatrix(int** mat, int n);
void printBoard(int** board, int n, int m);

// Расчет минимального числа ходов (математический)
unsigned int calcMinMovesHalf(int** board, int n, int m, int bestTopVal, vector<Point>& mismatches);

// Рекурсивный поиск минимальной стоимости спаривания
unsigned int findPairCost(const vector<Point>& mism, int idx);

// Симуляция ходов (по стратегии: идем по порядку и исправляем дефекты)
void simulateStepsHalf(int** board, int n, int m, int topVal);

#endif // FUNC_H