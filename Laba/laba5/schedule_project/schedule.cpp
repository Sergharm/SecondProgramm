#include "schedule.h"
#include <iostream>
#include <algorithm>

using namespace std;

ScheduleManager::ScheduleManager() : currentMonth(1), daysInMonth({
    {1, 31}, {2, 28}, {3, 31}, {4, 30},
    {5, 31}, {6, 30}, {7, 31}, {8, 31},
    {9, 30}, {10, 31}, {11, 30}, {12, 31}
}) {}

Command ScheduleManager::parseCommand(const string& cmd) const {
    if (cmd == "CLASS") return Command::CLASS;
    if (cmd == "NEXT") return Command::NEXT;
    if (cmd == "VIEW") return Command::VIEW;
    return Command::UNKNOWN;
}

int ScheduleManager::getDaysInCurrentMonth() const {
    auto it = daysInMonth.find(currentMonth);
    return (it != daysInMonth.end()) ? it->second : 31;
}

void ScheduleManager::addClass(int day, const string& discipline) {
    int days = getDaysInCurrentMonth();
    
    if (day < 1 || day > days) {
        cout << "Ошибка: В этом месяце всего " << days << " дней" << endl;
        return;
    }
    
    auto& dayDisciplines = schedule[currentMonth][day];
    
    for (const auto& d : dayDisciplines) {
        if (d == discipline) {
            cout << "Ошибка: Дисциплина " << discipline << " уже есть в этот день" << endl;
            return;
        }
    }
    
    dayDisciplines.push_back(discipline);
    cout << "Добавлена дисциплина " << discipline << " на день " << day << endl;
}

void ScheduleManager::nextMonth() {
    int prevMonth = currentMonth;
    currentMonth = (currentMonth % 12) + 1;
    
    cout << "Переход на следующий месяц" << endl;
    
    int newMonthLastDay = daysInMonth.at(currentMonth);
    
    map<int, vector<string>> prevData = schedule[prevMonth];
    schedule[currentMonth].clear();
    
    for (const auto& pair : prevData) {
        int day = pair.first;
        const vector<string>& disciplines = pair.second;
        
        int newDay = day;
        // Если день не существует в НОВОМ месяце — переносим на его последний день
        if (day > newMonthLastDay) {
            newDay = newMonthLastDay;
        }
        
        auto& targetDay = schedule[currentMonth][newDay];
        for (const auto& discipline : disciplines) {
            targetDay.push_back(discipline);
        }
    }
}

void ScheduleManager::viewDay(int day) const {
    int days = getDaysInCurrentMonth();
    
    if (day < 1 || day > days) {
        cout << "Ошибка: В этом месяце всего " << days << " дней. Добавим занятие на последний день" << endl;
        day = days;
        return;
    }
    
    auto monthIt = schedule.find(currentMonth);
    if (monthIt == schedule.end()) {
        cout << "В день " << day << " мы свободны!" << endl;
        return;
    }
    
    auto dayIt = monthIt->second.find(day);
    if (dayIt == monthIt->second.end() || dayIt->second.empty()) {
        cout << "В день " << day << " мы свободны!" << endl;
        return;
    }
    
    cout << "В день " << day << " занятия в университете: ";
    bool first = true;
    for (const auto& discipline : dayIt->second) {
        if (!first) cout << ", ";
        cout << discipline;
        first = false;
    }
    cout << endl;
}