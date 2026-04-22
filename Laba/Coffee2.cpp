#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>

using namespace std;

int coffee(double Tsr, double Tc, double HeatCoef, int t, vector<double>& Coffee);
double aproxA(vector<double> Coffee);
double aproxB(vector<double> Coffee, double a);
double Correl(vector<double> Coffee);

double Correl(vector<double> Coffee) {

    double sumTemp = 0; //сумма температур
    for (double T : Coffee) {
        sumTemp += T;
    }

    int len = Coffee.size(); //количество измерений
    double TMedium = sumTemp / len; //среднее значение всех измеренных значений
    double tMedium = (len - 1) * len / 2;
    double sumNumbers = 0; //арифметическая сумма значений произведения температуры и времени
    double tSumSquare = 0; //арифметическая сумма квадрата времени
    double TSumSquare = 0; //арифметическая сумма квадрата температуры

    for (int i = 0; i < len; i++) {
        sumNumbers += ((i - tMedium) * (Coffee[i] - TMedium));
        tSumSquare += ((i - tMedium) * (i - tMedium));
        TSumSquare += ((Coffee[i] - TMedium) * (Coffee[i] - TMedium));
    }
    return sumNumbers / sqrt(TSumSquare * tSumSquare);
}

int coffee(double Tsr, double Tc, double HeatCoef, int t, vector<double>& Coffee) {
    for (int i = 0; i <= t; i++) {
    Coffee.push_back(Tc); //добавляем изначальное значение
    Tc = Tc - HeatCoef * (Tc - Tsr); //высчитываем новое по формуле
 }
    return 0;
}

double aproxA(vector<double> Coffee) {

    double ET = 0, Et = 0, ETt = 0, Et2 = 0;
    int len = Coffee.size(); //количество измерений

    for (int i = 0; i < len; i++) {
        ET += Coffee[i]; //сумма по температуре
        Et += i; //сумма по времени

        ETt += Coffee[i] * i; //сумма для произведения по оси температуры и времени
        Et2 += i*i; //сумма для квадрата температуры
 }
    return (len * ETt - (Et * ET)) / (len * Et2 - Et * Et);
}

double aproxB(vector<double> Coffee, double a) {

    double ET = 0, Et = 0;
    int len = Coffee.size(); //количество измерений

    for (int i = 0; i < len; i++) {
    ET += Coffee[i]; //сумма по температуре
    Et += i; //сумма по времени
 }
    return (ET - a * Et) / len;
}

int main() {

    cout << "Введите температуру воздуха и кофе, коэффициент охлаждения," << endl;
    double Tsr, Tc, HeatCoef; //температура среды, температура кофе, коэф.остывания
    cin >> Tsr >> Tc >> HeatCoef;

    cout << "максимальное время охлаждение в минутах" << endl;
    int t; //время остывания
    cin >> t; 

    vector<double> Coffee;
    coffee(Tsr, Tc, HeatCoef, t, Coffee); //расчитываем температуру по времени

    cout << "Время" << "      " << "Температура кофе" << endl;
    int time = 0;
    for (auto temperature: Coffee) {
        cout << time << "      " << temperature << setprecision(4) << endl; //вывели все значение от 0 до времени
        time++;
 }

    double a = aproxA(Coffee); // в a помещаем отклонение по оси времени
    double b = aproxB(Coffee, a); //в b помещаем отклонение по оси температуры
    double correl = Correl(Coffee); //высчитываем коэф.корреляции

    cout << endl << "Линия апроксимации: " << "T = " << a << setprecision(3) << " * t + " << b << setprecision(4) << endl; //линия апроксимации
    cout << endl << "Коэффициент корреляции " << correl << setprecision(2) << endl; 
}
