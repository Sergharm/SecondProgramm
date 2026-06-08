#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

// Функция расчёта температуры кофе по времени
void calculateCoffeeTemp(double envTemp, double initTemp, double heatCoef, int maxTime, vector<double>& temps);

// Метод наименьших квадратов: нахождение коэффициента a
double calculateSlopeA(const vector<double>& temps);

// Метод наименьших квадратов: нахождение коэффициента b
double calculateInterceptB(const vector<double>& temps, double slopeA);

// Расчёт коэффициента корреляции Пирсона
double calculateCorrelation(const vector<double>& temps);

double calculateCorrelation(const vector<double>& temps) {
    double sumTemp = 0;
    for (double t : temps) {
        sumTemp += t;
    }

    int n = temps.size();
    double meanTemp = sumTemp / n;
    double meanTime = (n - 1.0) / 2.0;
    
    double covSum = 0, timeVar = 0, tempVar = 0;

    for (int i = 0; i < n; i++) {
        double dt = i - meanTime;
        double dT = temps[i] - meanTemp;
        covSum += dt * dT;
        timeVar += dt * dt;
        tempVar += dT * dT;
    }
    
    return covSum / sqrt(timeVar * tempVar);
}

void calculateCoffeeTemp(double envTemp, double initTemp, double heatCoef, int maxTime, vector<double>& temps) {
    double currentTemp = initTemp;
    for (int i = 0; i <= maxTime; i++) {
        temps.push_back(currentTemp);
        currentTemp = currentTemp - heatCoef * (currentTemp - envTemp);
    }
}

double calculateSlopeA(const vector<double>& temps) {
    double sumT = 0, sumt = 0, sumTt = 0, sumt2 = 0;
    int n = temps.size();

    for (int i = 0; i < n; i++) {
        sumT += temps[i];
        sumt += i;
        sumTt += temps[i] * i;
        sumt2 += i * i;
    }
    
    return (n * sumTt - sumt * sumT) / (n * sumt2 - sumt * sumt);
}

double calculateInterceptB(const vector<double>& temps, double slopeA) {
    double sumT = 0, sumt = 0;
    int n = temps.size();

    for (int i = 0; i < n; i++) {
        sumT += temps[i];
        sumt += i;
    }
    
    return (sumT - slopeA * sumt) / n;
}

int main() {
    cout << "Введите температуру воздуха и кофе, коэффициент охлаждения:" << endl;
    double envTemp, initTemp, heatCoef;
    cin >> envTemp >> initTemp >> heatCoef;

    cout << "Максимальное время охлаждения в минутах:" << endl;
    int maxTime;
    cin >> maxTime; 

    vector<double> coffeeTemps;
    calculateCoffeeTemp(envTemp, initTemp, heatCoef, maxTime, coffeeTemps);

    cout << fixed << setprecision(4);
    cout << "\nВремя" << "\t" << "Температура кофе" << endl;
    cout << "---------------------------" << endl;
    
    for (size_t i = 0; i < coffeeTemps.size(); i++) {
        cout << i << "\t" << coffeeTemps[i] << endl;
    }

    double slopeA = calculateSlopeA(coffeeTemps);
    double interceptB = calculateInterceptB(coffeeTemps, slopeA);
    double correlation = calculateCorrelation(coffeeTemps);

    cout << fixed << setprecision(3);
    cout << "\nЛиния аппроксимации: T = " << slopeA << " * t + " << interceptB << endl;
    cout << fixed << setprecision(4);
    cout << "Коэффициент корреляции: " << correlation << endl; 
    
    return 0;
}