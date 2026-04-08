#include <iostream>
#include <limits>
using namespace std;

int sumOfDigits(int num) {
    int sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

int safeInput(const string& message) {
    int value;
    while (true) {
        cout << message;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Нужно ввести ЧИСЛО. Попробуйте ещё раз!" << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return value;
        }
    }
}

int main() {
    int N;
    
    // Ввод N с проверкой на положительное число
    do {
        N = safeInput("Введите N натуральных чисел: ");
        if (N <= 0) {
            cout << "Ошибка! N должно быть больше 0. Попробуйте ещё раз!" << endl;
        }
    } while (N <= 0);
    
    int count = 0;
    
    for (int i = 0; i < N; i++) {
        int num = safeInput("Введите число " + to_string(i + 1) + ": ");
        
        if (sumOfDigits(num) > 10) {
            count++;
        }
    }
    
    cout << "Количество чисел сумма которых больше 10 равна " << count << endl;
    return 0;
}