#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <locale>
using namespace std;

double f(double x) {
    if (x <= 0) return NAN;
    return pow(x - 6, 2) + log(x) - 5;
}

double df(double x) {
    if (x <= 0) return NAN;
    return 2 * (x - 6) + 1.0 / x;
}

double phi(double x, bool usePlus) {
    if (x <= 0) return NAN;
    double val = 5 - log(x);
    if (val < 0) return NAN;
    return usePlus ? 6 + sqrt(val) : 6 - sqrt(val);
}

void bisectionMethod(double a, double b, double eps) {
    vector<double> aVec, bVec, diffVec;
    
    while (abs(b - a) > eps) {
        aVec.push_back(a);
        bVec.push_back(b);
        diffVec.push_back(abs(b - a));
        
        double c = (a + b) / 2.0;
        if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    
    cout << "N     an          bn          bn-an" << endl;
    for (size_t i = 0; i < aVec.size(); ++i) {
        cout << fixed << setprecision(6);
        cout << i << "     " << aVec[i] << "    " << bVec[i] << "    " << diffVec[i] << endl;
    }
    cout << "Корень: " << (a + b) / 2.0 << endl << endl;
}

void newtonMethod(double x0, double eps) {
    vector<double> xnVec, xnextVec, diffVec;
    double x = x0;
    
    for (int i = 0; i < 100; ++i) {
        double fx = f(x);
        double dfx = df(x);
        if (abs(dfx) < 1e-10) break;
        
        double xNext = x - fx / dfx;
        double diff = abs(xNext - x);
        
        xnVec.push_back(x);
        xnextVec.push_back(xNext);
        diffVec.push_back(diff);
        
        x = xNext;
        if (diff < eps) break;
    }
    
    cout << "N     xn           xn+1        |xn+1-xn|" << endl;
    for (size_t i = 0; i < xnVec.size(); ++i) {
        cout << fixed << setprecision(6);
        cout << i << "     " << xnVec[i] << "    " << xnextVec[i] << "    " << diffVec[i] << endl;
    }
    cout << "Корень: " << x << endl << endl;
}

void simpleIterationMethod(double x0, double eps, bool usePlus) {
    vector<double> xnVec, xnextVec, diffVec;
    double x = x0;
    
    for (int i = 0; i < 100; ++i) {
        double xNext = phi(x, usePlus);
        if (isnan(xNext)) break;
        
        double diff = abs(xNext - x);
        xnVec.push_back(x);
        xnextVec.push_back(xNext);
        diffVec.push_back(diff);
        
        x = xNext;
        if (diff < eps) break;
    }
    
    cout << "N     xn          xn+1        |xn+1-xn|" << endl;
    for (size_t i = 0; i < xnVec.size(); ++i) {
        cout << fixed << setprecision(6);   
        cout << i << "     " << xnVec[i] << "    " << xnextVec[i] << "    " << diffVec[i] << endl;
    }
    cout << "Корень: " << x << endl << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    double eps = 1e-4;
    
    cout << "ПЕРВЫЙ КОРЕНЬ (интервал [4, 5]) " << endl;
    
    cout << "Метод половинного деления" << endl;
    bisectionMethod(4.0, 5.0, eps);
    
    cout << "Метод Ньютона" << endl;
    newtonMethod(4.5, eps);
    
    cout << "Метод простых итераций (ветвь с минусом)" << endl;
    simpleIterationMethod(4.5, eps, false);

    cout << "ВТОРОЙ КОРЕНЬ (интервал [7, 8])" << endl;
    
    cout << "Метод половинного деления" << endl;
    bisectionMethod(7.0, 8.0, eps);
    
    cout << "Метод Ньютона" << endl;
    newtonMethod(7.5, eps);
    
    cout << "Метод простых итераций (ветвь с плюсом)" << endl;
    simpleIterationMethod(7.5, eps, true);
    
    return 0;
}