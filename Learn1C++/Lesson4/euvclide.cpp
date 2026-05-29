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


int gcdExtended(int a, int b, int& x, int& y)
{
    // Блок У
    int x1 = a;
    int x2 = 1;
    int x3 = 0;
    // Блок Х
    int y1 = b;
    int y2 = 0;
    int y3 = 1;
    while (y1 != 0)
    {
        int Q = x1 / y1;

        int t1 = Q;
        int t2 = x2 - Q * y2;
        int t3 = x3 - Q * y3;
        
        x1 = y1;
        x2 = y3;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
    x = x2;
    y = x3;
    return x1;
}
