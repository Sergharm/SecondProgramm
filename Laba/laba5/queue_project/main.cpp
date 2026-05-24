#include "queue.h"
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
    
    vector<Window> windows;
    vector<Ticket> queue;
    int ticketCounter = 0;
    
    int windowCount;
    cout << "Введите количество окон:" << endl;
    cout << ">>> ";
    cin >> windowCount;
    
    cin.ignore();
    
    initQueue(windows, windowCount);
    
    cout << "Команды: ENQUEUE <время>, DISTRIBUTE, EXIT" << endl << endl;
    
    string line;
    while (true) {
        cout << ">>> ";
        if (!getline(cin, line)) {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        
        istringstream iss(line);
        string cmd;
        iss >> cmd;
        cmd = toUpper(cmd);
        
        if (cmd == "ENQUEUE") {
            int duration;
            if (iss >> duration) {
                if (duration > 0) {
                    enqueue(queue, ticketCounter, duration);
                } else {
                    cout << "Ошибка: время должно быть больше 0" << endl;
                }
            } else {
                cout << "Ошибка: ENQUEUE <время_в_минутах>" << endl;
            }
        }
        else if (cmd == "DISTRIBUTE") {
            if (queue.empty()) {
                cout << "Ошибка: Очередь пуста" << endl;
            } else {
                distribute(queue, windows);
                showWindows(windows);
            }
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