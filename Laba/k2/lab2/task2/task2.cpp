#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "set.h"
#include "storage.h"
#include <locale>

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string fileName, query;
    for (int i = 1; i < argc; i++) {
        string a = argv[i];
        if (a == "--file" && i + 1 < argc) fileName = argv[++i];
        else if (a == "--query" && i + 1 < argc) query = argv[++i];
    }
    if (fileName.empty() || query.empty()) {
        cout << "Запуск: task2 --file set.data --query \"SETADD имя элемент\"" << endl;
        return 1;
    }
    istringstream qs(query);
    string cmd, name, arg;
    qs >> cmd >> name >> arg;
    if (name.empty()) {
        cout << "Ошибка: укажите имя множества" << endl;
        return 1;
    }

    vector<string> lines = loadLines(fileName);
    string type;
    vector<string> elems;
    int idx = findName(lines, name, type, elems);
    if (idx == -1 && cmd != "SETADD") {
        cout << "Ошибка: множество " << name << " не найдено" << endl;
        return 1;
    }

    Set set;
    for (const auto& e : elems) set.Add(e);

    bool changed = false;
    if (cmd == "SETADD") {
        if (set.Add(arg)) { cout << "-> " << arg << endl; changed = true; }
        else cout << "Ошибка: элемент уже есть во множестве" << endl;
    } else if (cmd == "SETDEL") {
        if (set.Del(arg)) { cout << "-> " << arg << endl; changed = true; }
        else cout << "Ошибка: элемента нет во множестве" << endl;
    } else if (cmd == "SET_AT") {
        cout << (set.Contains(arg) ? "-> TRUE" : "-> FALSE") << endl;
    } else if (cmd == "PRINT") {
        if (set.Size() == 0) cout << "ПУСТОЕ" << endl;
        else {
            for (const auto& x : set.Items()) cout << x << " ";
            cout << endl;
        }
    } else {
        cout << "Ошибка: неизвестная команда " << cmd << endl;
        return 1;
    }

    if (changed) {
        putLine(lines, idx, "N", name, set.Items());
        if (!saveLines(fileName, lines)) {
            cout << "Ошибка записи файла " << fileName << endl;
            return 1;
        }
    }
    return 0;
}
