#include "utils.h"

void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

long long get_ll(const string& prompt, long long min_val, long long max_val) {
    long long val;
    while (true) {
        cout << prompt;
        if (!(cin >> val)) {
            cout << "Ошибка: введено не число. Попробуйте снова.\n";
            clear_input();
            continue;
        }
        if (val < min_val || val > max_val) {
            cout << "Ошибка: число должно быть в диапазоне [" 
                 << min_val << ", " << max_val << "].\n";
        } else {
            clear_input();
            return val;
        }
    }
}

string get_str(const string& prompt) {
    string s;
    cout << prompt;
    cin >> s;
    clear_input();
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