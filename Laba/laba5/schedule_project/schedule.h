#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <map>
#include <vector>

using namespace std;

enum class Command {
    CLASS,
    NEXT,
    VIEW,
    UNKNOWN
};

class ScheduleManager {
private:
    map<int, map<int, vector<string>>> schedule;
    int currentMonth;
    const map<int, int> daysInMonth;
    
public:
    ScheduleManager();
    Command parseCommand(const string& cmd) const;
    void addClass(int day, const string& discipline);
    void nextMonth();
    void viewDay(int day) const;
    int getCurrentMonth() const { return currentMonth; }
    int getDaysInCurrentMonth() const;
};

#endif