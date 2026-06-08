    #include <iostream>
    using namespace std;

    // Функция для подсчёта суммы цифр числа
    int sumOfDigits(int num) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;  // Добавляем последнюю цифру
            num /= 10;         // Убираем последнюю цифру
        }
        return sum;
    }

    int main() {
        int N;
        cout << "Введите N натуральных чисел: ";
        cin >> N;
        
        int count = 0;  // Счётчик чисел с суммой цифр > 10
        
        for (int i = 0; i < N; i++) {
            int num;
            cin >> num;
            
            if (sumOfDigits(num) > 10) {
                count++;
            }
        }
        
        cout << "Количество чисел сумма которых больше 10 равна " << count << endl;
        return 0;
    }