<<<<<<< HEAD
#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
=======
#include "utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdint>
>>>>>>> d6e8ed2 (Изменения во второй практике)

using namespace std;

void clear_input() {
<<<<<<< HEAD
    cin.clear(); 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
}

uint64_t get_ll(const string& podskazka, uint64_t minZnachenie, uint64_t maxZnachenie) {
    uint64_t znachenie;
=======
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

uint64 get_ll(const string& podskazka, uint64 minZnachenie, uint64 maxZnachenie) {
    uint64 znachenie;
>>>>>>> d6e8ed2 (Изменения во второй практике)
    while (true) {
        cout << podskazka;
        if (!(cin >> znachenie)) {
            cout << "Ошибка: введено не число. Попробуйте снова.\n";
<<<<<<< HEAD
            clear_input(); 
=======
            clear_input();
>>>>>>> d6e8ed2 (Изменения во второй практике)
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
<<<<<<< HEAD
    
=======
>>>>>>> d6e8ed2 (Изменения во второй практике)
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
<<<<<<< HEAD
            cout << "Ошибка: файл '" << fname << "' не найден или нет доступа.\n";
=======
            cout << "Ошибка: файл '" << fname << "' не найден.\n";
>>>>>>> d6e8ed2 (Изменения во второй практике)
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
<<<<<<< HEAD
}
=======
}
>>>>>>> d6e8ed2 (Изменения во второй практике)
