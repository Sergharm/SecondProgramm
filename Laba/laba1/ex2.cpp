#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <locale>
using namespace std; // Объявление использования пространства имён std
int main()
{
     setlocale(LC_ALL, "ru_RU.UTF-8");
    map<char, int> romeNumbers = // Контейнер для хранения базовых римских цифр
    {
        {'I', 1}, {'V', 5},
        {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500},
        {'M', 1000}
    };
    cout << "Для выхода из программы введите: Exit\n";
    string romeNumber; // Инициализация для хранения римской цифры полученной от пользователя
    while (true) // Бесконечный цикл обработки ввода и вывода результата
    {
        cout << "Введите римскую цифру заглавными буквами: ";
        getline(cin, romeNumber); // Получаем римскую цифру от пользователя
        if (romeNumber.empty()) // Проверка ввода
        {
            cout << "Пустой ввод!!\n";
            continue;
        }
        if (romeNumber == "Exit") break; // Условие выхода из цикла
        int arabicNumber = 0; // Инициализируем переменную для хранения арабской цифры
        for (size_t i = 0; i < (romeNumber.length()); i++) // Цикл для прохождения по строке
        {
            if (i+1 < romeNumber.length() && romeNumber[i] == 'I' && romeNumber[i+1] == 'V') // Условие на соответсвие цифре 4
            {
                arabicNumber += romeNumbers['V'] - romeNumbers['I'];
                i++;
            }
            else if (i+1 < romeNumber.length() && romeNumber[i] == 'I' && romeNumber[i+1] == 'X') // Условие на соответсвие цифре 9
            {
                arabicNumber += romeNumbers['X'] - romeNumbers['I'];
                i++;
            }
            else if (i+1 < romeNumber.length() && romeNumber[i] == 'X' && romeNumber[i+1] == 'L') // Условие на соответсвие числу 40
            {
                arabicNumber += romeNumbers['L'] - romeNumbers['X'];
                i++;
            }
            else if (i+1 < romeNumber.length() && romeNumber[i] == 'X' && romeNumber[i+1] == 'C')// Условие на соответсвие числу 90
            {
                arabicNumber += romeNumbers['C'] - romeNumbers['X'];
                i++;
            }
            else if (i+1 < romeNumber.length() && romeNumber[i] == 'C' && romeNumber[i+1] == 'D') // Условие на соответсвие числу 400
            {
                arabicNumber += romeNumbers['D'] - romeNumbers['C'];
                i++;
            }
            else if (i+1 < romeNumber.length() && romeNumber[i] == 'C' && romeNumber[i+1] == 'M') // Условие на соответсвие числу 900
            {
                arabicNumber += romeNumbers['M'] - romeNumbers['C'];
                i++;
            }
            else {arabicNumber += romeNumbers[romeNumber[i]];} // Получение других цифр - числ на основе правила сложения 

        }
        cout << romeNumber << " = " << arabicNumber << endl;  // вывод результата   
    }
    return 0;
}