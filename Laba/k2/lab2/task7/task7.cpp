// Задание 7, вариант 1: кэш LRU.
// Ввод (stdin): сначала ёмкость cap и количество запросов Q, затем Q запросов:
//   SET x y — записать значение;  GET x — вернуть значение или -1.
// Вывод: значения всех GET через пробел.
#include <iostream>
#include <string>
#include <vector>
#include "lru.h"
#include "locale"

using namespace std;

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int capacity = 0, q = 0;
    if (!(cin >> capacity >> q)) {
        cout << "Ошибка: в начале ввода должны быть cap и Q" << endl;
        return 1;
    }
    LRU cache(capacity);
    vector<string> results;
    for (int i = 0; i < q; i++) {
        string op;
        cin >> op;
        if (op == "SET") {
            string x, y;
            cin >> x >> y;
            cache.Set(x, y);
        } else if (op == "GET") {
            string x;
            cin >> x;
            results.push_back(cache.Get(x));
        } else {
            cout << "Ошибка: неизвестный запрос " << op << endl;
            return 1;
        }
    }
    for (size_t i = 0; i < results.size(); i++) {
        if (i) cout << " ";
        cout << results[i];
    }
    cout << endl;
    return 0;
}
