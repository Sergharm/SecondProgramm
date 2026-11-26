// Билипоб использует хеш-таблицу из задания 6.1 (двойное хеширование) как кэш:
// повторно запрошенные числа берутся из таблицы
#include <iostream>
#include <string>
#include "hashtable.h"
#include <locale>

using namespace std;

static string toRoman(int n) {
    static const pair<int, const char*> table[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"},
    };
    string res;
    for (const auto& r : table) {
        while (n >= r.first) {
            res += r.second;
            n -= r.first;
        }
    }
    return res;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    OpenTable cache(16, true);
    int hits = 0;
    string line;
    while (getline(cin, line)) {
        if (line.find_first_not_of(" \t\r") == string::npos) continue;
        int n = 0;
        bool numeric = true;
        for (char c : line) {
            if (c >= '0' && c <= '9') n = n * 10 + (c - '0');
            else if (c != ' ' && c != '\r' && c != '\t') numeric = false;
        }
        if (!numeric || n < 1 || n > 3999) {
            cout << "Ошибка: число должно быть от 1 до 3999: " << line << endl;
            continue;
        }
        string key = to_string(n), roman;
        if (cache.Get(key, roman)) {
            hits++;
        } else {
            roman = toRoman(n);
            cache.Put(key, roman);
        }
        cout << roman << endl;
    }
    cout << "В кэше: " << cache.Count() << ", повторных запросов из кэша: " << hits << endl;
    return 0;
}
