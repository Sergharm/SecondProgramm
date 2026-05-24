#include "plane.h"
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
    // Настройка кодировки для русского языка
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    
    map<string, Plane> planes;
    map<string, set<string>> townPlanes;
    
    cout << "Команды: CREATE_PLANE, PLANES_FOR_TOWN, TOWNS_FOR_PLANE, PLANES, EXIT" << endl << endl;
    
    string line;
    while (true) {
        cout << ">>> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        
        istringstream iss(line);
        string cmdStr;
        iss >> cmdStr;
        cmdStr = toUpper(cmdStr);
        
        // Выход обрабатываем отдельно
        if (cmdStr == "EXIT") {
            break;
        }
        
        CommandType cmd = parseCommand(cmdStr);
        
        if (cmd == CommandType::CREATE_PLANE) {
            string name;
            int n;
            if (!(iss >> name >> n)) {
                cout << "Ошибка: CREATE_PLANE <name> <N> <town1> ..." << endl;
                continue;
            }
            
            vector<string> towns;
            string town;
            bool valid = true;
            for (int i = 0; i < n; ++i) {
                if (!(iss >> town)) {
                    cout << "Ошибка: Недостаточно городов" << endl;
                    valid = false;
                    break;
                }
                towns.push_back(town);
            }
            
            if (valid) {
                createPlane(planes, townPlanes, name, towns);
            }
        }
        else if (cmd == CommandType::PLANES_FOR_TOWN) {
            string town;
            if (iss >> town) {
                showPlanesForTown(townPlanes, town);
            } else {
                cout << "Ошибка: PLANES_FOR_TOWN <town>" << endl;
            }
        }
        else if (cmd == CommandType::TOWNS_FOR_PLANE) {
            string planeName;
            if (iss >> planeName) {
                showTownsForPlane(planes, townPlanes, planeName);
            } else {
                cout << "Ошибка: TOWNS_FOR_PLANE <plane>" << endl;
            }
        }
        else if (cmd == CommandType::SHOW_PLANES) {
            showAllPlanes(planes);
        }
        else {
            cout << "Неизвестная команда." << endl;
        }
    }
    
    return 0;
}