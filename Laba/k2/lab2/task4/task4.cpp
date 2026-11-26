#include <iostream>
#include <string>
#include "array.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


static void toArray(const string& s, Array& a) {
    a.Create();
    for (char c : s) {
        a.PushBack(string(1, c));
    }
}

bool match(Array& p, Array& s) {
    int i = 0, j = 0;
    int starP = -1, starS = 0;
    while (j < s.Size()) {
        if (i < p.Size() && p.At(i) == "*") {
            starP = i;
            starS = j;
            i++;
        } else if (i < p.Size() && (p.At(i) == "?" || p.At(i) == s.At(j))) {
            i++;
            j++;
        } else if (starP != -1) {
            i = starP + 1;   // «*» захватывает ещё один символ
            starS++;
            j = starS;
        } else {
            return false;
        }
    }
    while (i < p.Size() && p.At(i) == "*") i++; // хвостовые «*» могут быть пустыми
    return i == p.Size();
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc < 3) {
        cout << "Запуск: task4 \"шаблон\" \"строка\"" << endl;
        return 1;
    }
    Array p, s;
    toArray(argv[1], p);
    toArray(argv[2], s);
    cout << (match(p, s) ? "Соответствует" : "Не соответствует") << endl;
    return 0;
}
