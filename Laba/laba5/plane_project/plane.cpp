#include "plane.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Парсинг команды из строки
CommandType parseCommand(const string& cmd) {
    if (cmd == "CREATE_PLANE") return CommandType::CREATE_PLANE;
    if (cmd == "PLANES_FOR_TOWN") return CommandType::PLANES_FOR_TOWN;
    if (cmd == "TOWNS_FOR_PLANE") return CommandType::TOWNS_FOR_PLANE;
    if (cmd == "PLANES") return CommandType::SHOW_PLANES;
    return CommandType::UNKNOWN;
}

// Создание самолета
bool createPlane(map<string, Plane>& planes, map<string, set<string>>& townPlanes,
                 const string& name, const vector<string>& towns) {
    // 1. Проверка на существующее имя
    if (planes.find(name) != planes.end()) {
        cout << "Ошибка: Самолет с именем " << name << " уже создан" << endl;
        return false;
    }
    
    // 2. Минимум 2 города
    if (towns.size() < 2) {
        cout << "Ошибка: Самолет не может быть создан с менее чем двумя городами" << endl;
        return false;
    }
    
    // 3. Проверка на дубликаты городов
    set<string> uniqueTowns;
    for (const string& town : towns) {
        if (uniqueTowns.find(town) != uniqueTowns.end()) {
            cout << "Ошибка: Самолет не может быть создан с двумя одинаковыми городами" << endl;
            return false;
        }
        uniqueTowns.insert(town);
    }
    
    // Добавляем самолет
    Plane plane;
    plane.name = name;
    plane.towns = towns;
    planes[name] = plane;
    
    // Обновляем обратный индекс (город -> список самолетов)
    for (const string& town : towns) {
        townPlanes[town].insert(name);
    }
    
    cout << "Самолет " << name << " создан" << endl;
    return true;
}

// Вывод самолетов для города
void showPlanesForTown(const map<string, set<string>>& townPlanes, const string& town) {
    auto it = townPlanes.find(town);
    if (it == townPlanes.end() || it->second.empty()) {
        cout << "Ошибка: Город " << town << " не найден" << endl;
        return;
    }
    
    cout << "Самолеты над городом " << town << ": ";
    bool first = true;
    for (const string& plane : it->second) {
        if (!first) cout << ", ";
        cout << plane;
        first = false;
    }
    cout << endl;
}

// Вывод городов для самолета + другие самолеты
void showTownsForPlane(const map<string, Plane>& planes, const map<string, set<string>>& townPlanes,
                       const string& planeName) {
    auto it = planes.find(planeName);
    if (it == planes.end()) {
        cout << "Ошибка: Самолет " << planeName << " не найден" << endl;
        return;
    }
    
    const Plane& plane = it->second;
    cout << "Города самолета " << planeName << ":" << endl;
    
    for (const string& town : plane.towns) {
        cout << "  Город " << town << ": ";
        
        // Ищем другие самолеты в этом городе
        auto townIt = townPlanes.find(town);
        if (townIt != townPlanes.end()) {
            bool first = true;
            bool hasOther = false;
            
            for (const string& otherPlane : townIt->second) {
                if (otherPlane != planeName) {
                    if (!first) cout << ", ";
                    cout << otherPlane;
                    first = false;
                    hasOther = true;
                }
            }
            
            if (!hasOther) cout << "-";
        } else {
            cout << "-";
        }
        cout << endl;
    }
}

// Вывод всех самолетов
void showAllPlanes(const map<string, Plane>& planes) {
    if (planes.empty()) {
        cout << "Ошибка: Самолеты не найдены" << endl;
        return;
    }
    
    for (const auto& pair : planes) {
        const Plane& p = pair.second;
        cout << "Самолет " << p.name << ": ";
        bool first = true;
        for (const string& town : p.towns) {
            if (!first) cout << " ";
            cout << town;
            first = false;
        }
        cout << endl;
    }
}