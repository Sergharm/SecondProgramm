#include "utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
using namespace std;

// Функция полной очистки буфера при ошибках ввода
void clear_input() {
    cin.clear(); // Сбрасываем флаг ошибки потока
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем всё до конца строки
}

long long get_ll(const string& prompt, long long min_val, long long max_val) {
    long long val;
    while (true) {
        cout << prompt;
        if (!(cin >> val)) {
            cout << "Ошибка: введено не число. Попробуйте снова.\n";
            clear_input(); // Здесь очистка обязательна, так как ввод сломался
            continue;
        }
        
        if (val < min_val || val > max_val) {
            cout << "Ошибка: число должно быть в диапазоне [" 
                 << min_val << ", " << max_val << "].\n";
            // Если число считалось, но не подошло по диапазону, 
            // буфер чистить не нужно, просто идем на новую итерацию
        } else {
            // Перед успешным возвратом избавляемся от остатков символа '\n' в буфере,
            // чтобы последующие вызовы ввода (например, строк) не считывали пустую строку.
            string dummy;
            getline(cin, dummy); 
            return val;
        }
    }
}

string get_str(const string& prompt) {
    string s;
    cout << prompt;
    cin >> s;
    
    // Вместо жесткого clear_input() аккуратно считываем остаток строки, 
    // чтобы не блокировать ввод, если буфер уже пуст.
    string dummy;
    getline(cin, dummy);
    return s;
}

string get_existing_file(const string& prompt) {
    string fname;
    while (true) {
        fname = get_str(prompt);
        ifstream test(fname);
        if (!test) {
            cout << "Ошибка: файл '" << fname << "' не найден или нет доступа.\n";
        } else {
            test.close();
            return fname;
        }
    }
}

string get_new_file(const string& prompt) {
    string fname;
    while (true) {
        fname = get_str(prompt);
        if (fname.empty()) {
            cout << "Имя файла не может быть пустым.\n";
        } else {
            return fname;
        }
    }
}
