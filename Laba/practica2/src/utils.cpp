#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

void clear_input() {
    cin.clear(); 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
}

uint64_t get_ll(const string& podskazka, uint64_t minZnachenie, uint64_t maxZnachenie) {
    uint64_t znachenie;
    while (true) {
        cout << podskazka;
        if (!(cin >> znachenie)) {
            cout << "Ошибка: введено не число. Попробуйте снова.\n";
            clear_input(); 
            continue;
        }
        
        if (znachenie < minZnachenie || znachenie > maxZnachenie) {
            cout << "Ошибка: число должно быть в диапазоне [" 
                 << minZnachenie << ", " << maxZnachenie << "].\n";
        } else {
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
