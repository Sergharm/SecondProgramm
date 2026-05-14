#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <locale>


using namespace std;


double f(double x) { // исходная функция f(x)
    return cos(x + 0.5) - x - 2;
}


double df(double x) { // производная f(x) для метода Ньютона
    return -sin(x + 0.5) - 1;
}


double phi(double x) { // функция x = phi(x) для метода итераций
    return cos(x + 0.5) - 2;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    double eps = 0.0001;
    cout << fixed << setprecision(5);

    
    cout << "---Метод половинного деления---" << endl;
    vector<double> aVec, bVec, diffVec;
    double a = -2.0, b = -1.0;

    while (abs(b - a) > eps) {
        aVec.push_back(a);
        bVec.push_back(b);
        diffVec.push_back(abs(b - a));
        
        double c = (a + b) / 2.0;
        if (f(a) * f(c) < 0) b = c;
        else a = c;
    }

    cout << "N     an          bn          bn-an" << endl;
    for (int i = 0; i < aVec.size(); i++) {
        cout << i << "     " << aVec[i] << "    " << bVec[i] << "    " << diffVec[i] << endl;
    }
    cout << "Корень: " << (a + b) / 2.0 << endl << endl;


    
    cout << "--- Метод Ньютона ---" << endl;
    vector<double> xn_newton, xnext_newton, diff_newton;
    double xNewt = -1.5;

    for (int i = 0; i < 100; i++) {
        double x_next = xNewt - f(xNewt) / df(xNewt);
        double d = abs(x_next - xNewt);
        
        xn_newton.push_back(xNewt);
        xnext_newton.push_back(x_next);
        diff_newton.push_back(d);

        xNewt = x_next;
        if (d < eps) break;
    }

    cout << "N     xn          xn+1        |xn+1-xn|" << endl;
    for (int i = 0; i < xn_newton.size(); i++) {
        cout << i << "     " << xn_newton[i] << "    " << xnext_newton[i] << "    " << diff_newton[i] << endl;
    }
    cout << "Корень: " << xNewt << endl << endl;


   
    cout << "--- Метод простых итераций ---" << endl;
    vector<double> xnIter, xnext_iter, diffIter;
    double xIt = -1.5;

    for (int i = 0; i < 100; i++) {
        double x_next = phi(xIt);
        double d = abs(x_next - xIt);

        xnIter.push_back(xIt);
        xnext_iter.push_back(x_next);
        diffIter.push_back(d);

        xIt = x_next;
        if (d < eps) break;
    }

    cout << "N     xn          xn+1        |xn+1-xn|" << endl;
    for (int i = 0; i < xnIter.size(); i++) {
        cout << i << "     " << xnIter[i] << "    " << xnext_iter[i] << "    " << diffIter[i] << endl;
    }
    cout << "Корень: " << xIt << endl;

    return 0;
}
