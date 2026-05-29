#include "warehouse.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <clocale>
#include <windows.h>

using namespace std;

string toUpper(string s) {
    for (char& c : s) {
        c = toupper(c);
    }
    return s;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    
    map<string, Cell> warehouse;
    initWarehouse(warehouse);

    cout << "=== Учет товаров (Вариант 3) ===" << endl;
    cout << "Команды: ADD, REMOVE, INFO, EXIT" << endl << endl;

    string line;
    while (true) {
        cout << ">>> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        istringstream iss(line);
        string cmd;
        iss >> cmd;
        cmd = toUpper(cmd);

        if (cmd == "ADD") {
            string prod, addr; 
            int qty;
            if (iss >> prod >> qty >> addr) {
                if (qty < 0){
                cout << "Ошибка. Нужно использовать положительное число" << endl;
                return 1;
            }
                addProduct(warehouse, prod, qty, addr);
            } else {
                cout << "Ошибка: ADD <товар> <кол-во> <адрес>" << endl;
            }
        }
        else if (cmd == "REMOVE") {
            string prod, addr; 
            int qty;
            if (iss >> prod >> qty >> addr) {
                if (qty < 0){
                cout << "Ошибка. Нужно использовать положительное число" << endl;
                return 1;
            }
                removeProduct(warehouse, prod, qty, addr);
            } else {
                cout << "Ошибка: REMOVE <товар> <кол-во> <адрес>" << endl;
            }
        }
        else if (cmd == "INFO") {
            showInfo(warehouse);
        }
        else if (cmd == "EXIT") {
            break;
        }
        else {
            cout << "Неизвестная команда" << endl;
        }
    }
    return 0;
}