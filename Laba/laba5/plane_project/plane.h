#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

// Структура самолета
struct Plane {
    string name;
    vector<string> towns;
};

// Перечисление команд
enum class CommandType {
    CREATE_PLANE,
    PLANES_FOR_TOWN,
    TOWNS_FOR_PLANE,
    SHOW_PLANES,
    UNKNOWN
};

// Объявление функций
CommandType parseCommand(const string& cmd);
bool createPlane(map<string, Plane>& planes, map<string, set<string>>& townPlanes, 
                 const string& name, const vector<string>& towns);
void showPlanesForTown(const map<string, set<string>>& townPlanes, const string& town);
void showTownsForPlane(const map<string, Plane>& planes, const map<string, set<string>>& townPlanes, 
                       const string& planeName);
void showAllPlanes(const map<string, Plane>& planes);

#endif