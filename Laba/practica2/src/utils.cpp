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

uint get_ll(const string& podskazka, uint minZnachenie, uint maxZnachenie) {
    uint znachenie;
    while (true) {
        cout << podskazka;
        if (!(cin >> znachenie)) {
            cout << "Ошибка: введено не число. Попробуйте снова.\n";
            clear_input(); // Здесь очистка обязательна, так как ввод сломался
            continue;
        }
        
        if (znachenie < minZnachenie || znachenie > maxZnachenie) {
            cout << "Ошибка: число должно быть в диапазоне [" 
                 << minZnachenie << ", " << maxZnachenie << "].\n";
            // Если число считалось, но не подошло по диапазону, 
            // буфер чистить не нужно, просто идем на новую итерацию
        } else {
            // Перед успешным возвратом избавляемся от остатков символа '\n' в буфере,
            string pustishka;
            getline(cin, pustishka); 
            return znachenie;
        }
    }
}

string get_str(const string& podskazka) {
    string s;
    cout << podskazka;
    cin >> s;
    

    string pustishka;
    getline(cin, pustishka);
    return s;
}

string get_existing_file(const string& podskazka) {
    string fname;
    while (true) {
        fname = get_str(podskazka);
        ifstream test(fname);
        if (!test) {
            cout << "Ошибка: файл '" << fname << "' не найден или нет доступа.\n";
        } else {
            test.close();
            return fname;
        }
    }
}

string get_new_file(const string& podskazka) {
    string fname;
    while (true) {
        fname = get_str(podskazka);
        if (fname.empty()) {
            cout << "Имя файла не может быть пустым.\n";
        } else {
            return fname;
        }
    }
}