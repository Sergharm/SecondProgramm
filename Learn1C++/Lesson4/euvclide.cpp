#include <iostream>
#include <locale>

int extEuclid(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1; 
    int d = extEuclid(b, a % b, x1, y1);

    x = y1;
    y = x1 - y1 * (a / b);

    return d; 
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int a = 500, b = 145;
    int x, y;
    
    int gcd = extEuclid(a, b, x, y);

    std::cout << "НОД(" << a << ", " << b << ") = " << gcd << "\n";
    std::cout << "Коэффициенты: x = " << x << ", y = " << y << "\n";
    std::cout << "Проверка: " << a << "*(" << x << ") + " << b << "*(" << y << ") = " 
              << (a * x + b * y) << "\n";

    return 0;
}
