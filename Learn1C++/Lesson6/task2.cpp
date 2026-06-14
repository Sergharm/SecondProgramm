#include <iostream>
#include <string>
#include <clocale>
using namespace std;

struct Student{
    string name;
    int group_id;
    double grades[5];
};


double calculate_average(Student s){
    double sum = 0;
    for(int i = 0; i < 5; i++){
        sum += s.grades[i];
    }
    return sum / 5;
}



Student find_best_student(Student students[], int count){
    int best_student = 0;
    double max_average = calculate_average(students[0]);
    for (int i = 1; i < count; i++){
        double current_average = calculate_average(students[i]);
        if (current_average > max_average){
            max_average = current_average;
            best_student = i;
        }
    }
    return students[best_student];
}

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Student students[3] = {
        {"Иванов", 101, {5, 4, 5, 5, 4}},
        {"Петров", 102, {3, 4, 3, 4, 3}},
        {"Сидоров", 101, {5, 5, 5, 5, 5}},
    };
    Student best = find_best_student(students, 3);
    cout << "Лучший студент: " << best.name << endl;
    cout << "Группа: " << best.group_id << endl;
    cout << "Средний балл: " << calculate_average(best) << endl;
    return 0;
}