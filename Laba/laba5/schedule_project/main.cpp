#include "schedule.h"
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
    
    ScheduleManager manager;
    
    int n;
    cout << "Введите количество запросов(N):" << endl;
    cout << ">>> ";
    cin >> n;
    cin.ignore();
    
    cout << endl;
    
    string line;
    int count = 0;
    
    while (count < n && getline(cin, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string cmdStr;
        iss >> cmdStr;
        cmdStr = toUpper(cmdStr);
        
        Command cmd = manager.parseCommand(cmdStr);
        
        if (cmd == Command::CLASS) {
            int day;
            string discipline;
            if (iss >> day >> discipline) {
                manager.addClass(day, discipline);
            } else {
                cout << "Ошибка: CLASS <день> <дисциплина>" << endl;
            }
        }
        else if (cmd == Command::NEXT) {
            manager.nextMonth();
        }
        else if (cmd == Command::VIEW) {
            int day;
            if (iss >> day) {
                manager.viewDay(day);
            } else {
                cout << "Ошибка: VIEW <день>" << endl;
            }
        }
        else {
            cout << "Неизвестная команда" << endl;
        }
        
        count++;
    }
    
    return 0;
}