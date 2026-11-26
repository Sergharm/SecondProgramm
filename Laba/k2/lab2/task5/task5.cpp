#include <iostream>
#include <memory>
#include <string>
#include "array.h"
#include "avl.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    Array seq;
    string tok;
    bool terminated = false;

    while (cin >> tok) {
        int v;
        try {
            size_t pos = 0;
            v = stoi(tok, &pos);
            if (pos != tok.size()) {
                cout << "Ошибка: ожидаются целые числа" << endl;
                return 1;
            }
        } catch (...) {
            cout << "Ошибка: ожидаются целые числа" << endl;
            return 1;
        }
        if (v == 0) {
            terminated = true;
            break;
        }
        seq.PushBack(tok);
    }

    if (!terminated) {
        cout << "Ошибка: последовательность должна оканчиваться нулём" << endl;
        return 1;
    }

    unique_ptr<AvlNode> root;
    for (int i = 0; i < seq.Size(); i++) {
        insertKey(root, stoi(seq.At(i)));
    }

    bool ok = true;
    heightAndCheck(root.get(), ok);
    cout << (ok ? "YES" : "NO") << endl;
    return 0;
}
