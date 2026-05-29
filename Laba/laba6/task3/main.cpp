#include "solver.h"

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");


    double M = 1.08;
    double N = 0.22;
    double P = -1.16;

    Matrix A = {
        { M,   -0.04,  0.21, -1.16 },
        { 0.25, -1.23,  N,   -0.09 },
        {-0.21,  0.22,  0.80, -0.13 },
        { 0.15, -1.31,  0.06,  P    }
    };

    Vector b = { -1.24, P, 2.56, M };

    cout << "Исходная система:" << endl;
    printMatrix(A, "Матрица A");
    printVector(b, "Вектор b");

    cout << "Прямой метод (LU-разложение)" << endl;
    Vector xDirect = solveDirectLU(A, b);
    printVector(xDirect, "Корни системы (LU)");

    cout << "Подготовка к итерационному методу" << endl;
    Vector f;
    Matrix C = toCanonicalForm(A, b, f);

    printMatrix(C, "Матрица C (каноническая форма)");
    printVector(f, "Вектор f");

    double norm = matrixNormInf(C);
    cout << fixed << setprecision(4);
    cout << "Норма матрицы C: " << norm << endl;
    if (norm < 1.0)
        cout << "Условие сходимости выполняется." << endl;
    else
        cout << "Условие сходимости не выполняется." << endl;

    cout << "Метод Зейделя" << endl;

    double eps = 1e-3;
    int maxIter = 1000;

    Vector xIter = gaussSeidel(C, f, eps, maxIter);
    printVector(xIter, "Корни системы (Зейдель)");

    cout << "Сравнение результатов" << endl;
    for (int i = 0; i < xDirect.size(); ++i) {
        double diff = fabs(xDirect[i] - xIter[i]);
        cout << "|x" << (i + 1) << "(LU) - x" << (i + 1) << "(GS)| = "
             << fixed << setprecision(6) << diff << endl;
    }

    return 0;
}