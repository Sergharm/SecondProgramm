#include "solver.h"
#include <cstdlib>

// LU-разложение методом Дулиттла
void luDecompose(const Matrix& A, Matrix& L, Matrix& U) {
    int n = A.size();
    L.assign(n, vector<double>(n, 0.0));
    U.assign(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) L[i][i] = 1.0;

    for (int k = 0; k < n; ++k) {
        for (int j = k; j < n; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) sum += L[k][p] * U[p][j];
            U[k][j] = A[k][j] - sum;
        }
        for (int i = k + 1; i < n; ++i) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) sum += L[i][p] * U[p][k];
            L[i][k] = (A[i][k] - sum) / U[k][k];
        }
    }
}

// Прямой ход: решение Ly = b
Vector solveForward(const Matrix& L, const Vector& b) {
    int n = b.size();
    Vector y(n);
    for (int i = 0; i < n; ++i) {
        double sum = 0.0;
        for (int j = 0; j < i; ++j) sum += L[i][j] * y[j];
        y[i] = b[i] - sum;
    }
    return y;
}

// Обратный ход: решение Ux = y
Vector solveBackward(const Matrix& U, const Vector& y) {
    int n = y.size();
    Vector x(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = 0.0;
        for (int j = i + 1; j < n; ++j) sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
    return x;
}

// Полный прямой метод с выводом промежуточных результатов
Vector solveDirectLU(const Matrix& A, const Vector& b) {
    Matrix L, U;
    luDecompose(A, L, U);

    cout << "Результаты LU-разложения:" << endl;
    printMatrix(L, "Матрица L");
    printMatrix(U, "Матрица U");

    Vector y = solveForward(L, b);
    cout << "Прямой ход (Ly = b):" << endl;
    printVector(y, "Вектор y");

    Vector x = solveBackward(U, y);
    cout << "Обратный ход (Ux = y):" << endl;
    return x;
}

// Преобразование к каноническому виду x = Cx + f
Matrix toCanonicalForm(const Matrix& A, const Vector& b, Vector& f) {
    int n = A.size();
    Matrix C(n, vector<double>(n, 0.0));
    f.resize(n);

    for (int i = 0; i < n; ++i) {
        double diag = A[i][i];
        if (fabs(diag) < 1e-10) {
            cerr << "Ошибка: нулевой диагональный элемент в строке " << i + 1 << endl;
            exit(1);
        }
        f[i] = b[i] / diag;
        for (int j = 0; j < n; ++j) {
            if (i != j) C[i][j] = -A[i][j] / diag;
        }
    }
    return C;
}

// Вычисление бесконечной нормы матрицы
double matrixNormInf(const Matrix& C) {
    double maxSum = 0.0;
    for (const auto& row : C) {
        double rowSum = 0.0;
        for (double val : row) rowSum += fabs(val);
        if (rowSum > maxSum) maxSum = rowSum;
    }
    return maxSum;
}

// Метод Зейделя
Vector gaussSeidel(const Matrix& C, const Vector& f, double eps, int maxIter) {
    int n = f.size();
    Vector x(n, 0.0);
    double err = 1.0;
    int iter = 0;

    cout << "Таблица итераций:" << endl;
    cout << setw(4) << "N" << " |";
    for (int i = 0; i < n; ++i) cout << setw(10) << ("x" + to_string(i + 1));
    cout << setw(10) << "eps" << endl;
    cout << string(10 + n * 10, '-') << endl;

    do {
        Vector x_old = x;
        err = 0.0;
        ++iter;

        for (int i = 0; i < n; ++i) {
            double sum = f[i];
            for (int j = 0; j < n; ++j) {
                if (i != j) sum += C[i][j] * x[j];
            }
            x[i] = sum;
        }

        for (int i = 0; i < n; ++i) {
            double diff = fabs(x[i] - x_old[i]);
            if (diff > err) err = diff;
        }

        cout << setw(4) << iter << " |";
        for (double val : x) cout << setw(10) << fixed << setprecision(6) << val;
        cout << setw(10) << scientific << setprecision(2) << err << endl;

        if (iter >= maxIter) {
            cout << "Достигнуто максимальное число итераций." << endl;
            break;
        }
    } while (err > eps);

    cout << "Метод сошёлся за " << iter << " итераций." << endl;
    return x;
}

// Форматированный вывод матрицы
void printMatrix(const Matrix& M, const string& name) {
    cout << name << ":" << endl;
    for (const auto& row : M) {
        for (double val : row)
            cout << fixed << setw(8) << setprecision(4) << val << " ";
        cout << endl;
    }
    cout << endl;
}

// Форматированный вывод вектора
void printVector(const Vector& v, const string& name) {
    cout << name << ": ";
    for (double val : v)
        cout << fixed << setw(8) << setprecision(4) << val << " ";
    cout << endl << endl;
}

// Вывод одной строки таблицы итераций
void printIterationTable(int iter, const Vector& x, double eps_val) {
    cout << setw(4) << iter << " |";
    for (double val : x)
        cout << setw(10) << fixed << setprecision(6) << val;
    cout << setw(10) << scientific << setprecision(2) << eps_val << endl;
}