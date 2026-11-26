#include <iostream>
#include <string>
#include "stack.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

static int priority(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

static bool isOperand(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool toPostfix(const string& expr, string& out, string& err) {
    out.clear();
    Stack ops;
    bool expectOperand = true; //

    for (char c : expr) {
        if (c == ' ') continue;

        if (isOperand(c)) {
            if (!expectOperand) {
                err = "пропущен оператор перед операндом";
                return false;
            }
            out += c;
            expectOperand = false;
        } else if (c == '(') {
            if (!expectOperand) {
                err = "пропущен оператор перед '('";
                return false;
            }
            ops.push("(");
            expectOperand = true;
        } else if (c == ')') {
            if (expectOperand) {
                err = "пустые скобки или пропущен операнд перед ')'";
                return false;
            }
            bool found = false;
            while (!ops.IsEmpty()) {
                string top = ops.Top();
                ops.pop();
                if (top == "(") { found = true; break; }
                out += top;
            }
            if (!found) {
                err = "лишняя закрывающая скобка";
                return false;
            }
            expectOperand = false;
        } else if (priority(c) > 0) {
            if (expectOperand) {
                err = string("пропущен операнд перед оператором '") + c + "'";
                return false;
            }
            while (!ops.IsEmpty()) {
                string top = ops.Top();
                if (top == "(" || priority(top[0]) < priority(c)) break;
                out += top;
                ops.pop();
            }
            ops.push(string(1, c));
            expectOperand = true;
        } else {
            err = string("недопустимый символ '") + c + "'";
            return false;
        }
    }

    if (expectOperand) {
        err = "выражение пустое или не хватает операнда в конце";
        return false;
    }

    while (!ops.IsEmpty()) {
        string top = ops.Top();
        ops.pop();
        if (top == "(") {
            err = "незакрытая скобка";
            return false;
        }
        out += top;
    }
    return true;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc < 2) {
        cout << "Запуск: task1 \"A+B*C\"" << endl;
        return 1;
    }
    string result, err;
    if (!toPostfix(argv[1], result, err)) {
        cout << "Ошибка: " << err << endl;
        return 1;
    }
    cout << result << endl;
    return 0;
}
