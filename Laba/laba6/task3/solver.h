#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <locale>
#include <windows.h>

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

// Прямой метод: LU-разложение
void luDecompose(const Matrix& A, Matrix& L, Matrix& U);
Vector solveForward(const Matrix& L, const Vector& b);
Vector solveBackward(const Matrix& U, const Vector& y);
Vector solveDirectLU(const Matrix& A, const Vector& b);

// Итерационный метод: Зейделя
Matrix toCanonicalForm(const Matrix& A, const Vector& b, Vector& f);
double matrixNormInf(const Matrix& C);
Vector gaussSeidel(const Matrix& C, const Vector& f, double eps, int maxIter);

// Утилиты вывода
void printMatrix(const Matrix& M, const string& name);
void printVector(const Vector& v, const string& name);
void printIterationTable(int iter, const Vector& x, double eps_val);