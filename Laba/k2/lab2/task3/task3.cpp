// Задание 3, вариант 3: дополнительные операции над множествами.
// Программа поддерживает все команды множества из задания 2 и новые операции:
//   SETADD имя x      добавить элемент
//   SETDEL имя x      удалить элемент
//   SET_AT имя x      проверить наличие элемента (TRUE/FALSE)
//   PRINT имя         вывести множество
//   UNION A B C       C = A ∪ B
//   INTERSECT A B C   C = A ∩ B
//   DIFF A B C        C = A \ B
// Запуск:
//   task3.exe --file set.data --query "UNION A B C"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "set.h"
#include "storage.h"
#include "locale"

using namespace std;

// читает множество по имени из строк файла; false, если его нет
static bool loadSet(const vector<string>& lines, const string& name, Set& s) {
    string type;
    vector<string> elems;
    if (findName(lines, name, type, elems) == -1) return false;
    for (const auto& e : elems) s.Add(e);
    return true;
}

// сохраняет множество под именем: заменяет строку или добавляет новую
static bool storeSet(vector<string>& lines, const string& name, Set& s, const string& fileName) {
    string type;
    vector<string> elems;
    int idx = findName(lines, name, type, elems);
    putLine(lines, idx, "N", name, s.Items());
    return saveLines(fileName, lines);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string fileName, query;
    for (int i = 1; i < argc; i++) {
        string a = argv[i];
        if (a == "--file" && i + 1 < argc) fileName = argv[++i];
        else if (a == "--query" && i + 1 < argc) query = argv[++i];
    }
    if (fileName.empty() || query.empty()) {
        cout << "Запуск: task3 --file set.data --query \"SETADD A x\" | \"SETDEL A x\" | \"SET_AT A x\" | \"PRINT A\" | \"UNION A B C\" | \"INTERSECT A B C\" | \"DIFF A B C\"" << endl;
        return 1;
    }

    vector<string> words;
    istringstream qs(query);
    string w;
    while (qs >> w) words.push_back(w);
    if (words.size() < 2) {
        cout << "Ошибка: неверный запрос" << endl;
        return 1;
    }
    const string cmd = words[0];
    vector<string> lines = loadLines(fileName);

    // ---- команды одного множества: SETADD, SETDEL, SET_AT, PRINT ----
    if (cmd == "SETADD" || cmd == "SETDEL" || cmd == "SET_AT" || cmd == "PRINT") {
        const string name = words[1];
        const string arg = words.size() >= 3 ? words[2] : "";
        Set s;
        bool exists = loadSet(lines, name, s);

        if (cmd == "SETADD") {
            if (arg.empty()) { cout << "Ошибка: укажите элемент" << endl; return 1; }
            if (!s.Add(arg)) { cout << "Ошибка: элемент уже есть во множестве" << endl; return 1; }
            cout << "-> " << arg << endl;
            if (!storeSet(lines, name, s, fileName)) { cout << "Ошибка записи файла" << endl; return 1; }
        } else if (!exists) {
            cout << "Ошибка: множество " << name << " не найдено" << endl;
            return 1;
        } else if (cmd == "SETDEL") {
            if (!s.Del(arg)) { cout << "Ошибка: элемента нет во множестве" << endl; return 1; }
            cout << "-> " << arg << endl;
            if (!storeSet(lines, name, s, fileName)) { cout << "Ошибка записи файла" << endl; return 1; }
        } else if (cmd == "SET_AT") {
            cout << (s.Contains(arg) ? "-> TRUE" : "-> FALSE") << endl;
        } else { // PRINT
            if (s.Size() == 0) cout << "ПУСТОЕ" << endl;
            else {
                for (const auto& x : s.Items()) cout << x << " ";
                cout << endl;
            }
        }
        return 0;
    }

    // ---- операции над двумя множествами: UNION, INTERSECT, DIFF ----
    if (words.size() != 4 || (cmd != "UNION" && cmd != "INTERSECT" && cmd != "DIFF")) {
        cout << "Ошибка: неизвестная команда или неверное число аргументов" << endl;
        return 1;
    }
    Set a, b;
    if (!loadSet(lines, words[1], a) || !loadSet(lines, words[2], b)) {
        cout << "Ошибка: одно из множеств не найдено" << endl;
        return 1;
    }
    Set res;
    if (cmd == "UNION") {
        for (const auto& x : a.Items()) res.Add(x);
        for (const auto& x : b.Items()) res.Add(x);
    } else if (cmd == "INTERSECT") {
        for (const auto& x : a.Items()) if (b.Contains(x)) res.Add(x);
    } else { // DIFF
        for (const auto& x : a.Items()) if (!b.Contains(x)) res.Add(x);
    }
    if (!storeSet(lines, words[3], res, fileName)) {
        cout << "Ошибка записи файла" << endl;
        return 1;
    }
    for (const auto& x : res.Items()) cout << x << " ";
    cout << endl;
    return 0;
}
