#include <iostream>
#include <vector>
#include <random>
#include <cstdint>
#include <string>

using namespace std;

// Алгоритм 1: "Око за око" (Tit-for-Tat)
// Начинает с сотрудничества, далее копирует последний ход противника
bool titForTat(int round_number, const vector<bool>& self_choices, const vector<bool>& enemy_choices) {
    if (round_number == 0) return true;
    return enemy_choices.back();
}

// Алгоритм 2: "Всегда предаёт" (Always Defect)
// Эгоистичная стратегия, никогда не сотрудничает
bool alwaysDefect(int round_number, const vector<bool>& self_choices, const vector<bool>& enemy_choices) {
    return false;
}

// Алгоритм 3: "Обидчивый" (Grudger)
// Сотрудничает, пока противник не предаст хотя бы раз, после чего всегда предаёт
bool grudger(int round_number, const vector<bool>& self_choices, const vector<bool>& enemy_choices) {
    for (bool choice : enemy_choices) { // Range-based for loop (C++11)
        if (!choice) return false;
    }
    return true;
}

// Тип для хранения указателя на функцию-стратегию
using Strategy = bool(*)(int, const vector<bool>&, const vector<bool>&);

// Проведение одного матча между двумя алгоритмами
void playMatch(const string& nameA, Strategy algA, const string& nameB, Strategy algB, 
               int& totalA, int& totalB) {
    random_device rd;
    mt19937 gen(rd());
    int rounds = 100 + gen() % 101; // Случайное кол-во раундов от 100 до 200

    vector<bool> histA, histB; // Контейнеры для истории выборов
    int scoreA = 0, scoreB = 0;

    for (int r = 0; r < rounds; ++r) {
        bool choiceA = algA(r, histA, histB);
        bool choiceB = algB(r, histB, histA);

        histA.push_back(choiceA);
        histB.push_back(choiceB);

        // Начисление очков согласно правилам
        if (choiceA && choiceB) {
            scoreA += 24; scoreB += 24;
        } else if (choiceA && !choiceB) {
            scoreB += 20;
        } else if (!choiceA && choiceB) {
            scoreA += 20;
        } else {
            scoreA += 4; scoreB += 4;
        }
    }

    totalA += scoreA;
    totalB += scoreB;
    cout << nameA << " " << scoreA << " : " << scoreB << " " << nameB 
         << " (раундов: " << rounds << ")" << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Накопительные очки для турнира
    int s1 = 0, s2 = 0, s3 = 0;
    string n1 = "Око за око", n2 = "Всегда предаёт", n3 = "Обидчивый";

    cout << "=== Дилемма заключённого: Турнир алгоритмов ===\n" << endl;
    
    // Запуск 3 игр попарно
    playMatch(n1, titForTat, n2, alwaysDefect, s1, s2);
    playMatch(n2, alwaysDefect, n3, grudger, s2, s3);
    playMatch(n3, grudger, n1, titForTat, s3, s1);

    cout << "\n=== Итоговый счёт ===" << endl;
    cout << n1 << ": " << s1 << " очков\n";
    cout << n2 << ": " << s2 << " очков\n";
    cout << n3 << ": " << s3 << " очков\n";

    // Определение победителя
    int maxScore = s1;
    string winner = n1;
    if (s2 > maxScore) { maxScore = s2; winner = n2; }
    if (s3 > maxScore) { maxScore = s3; winner = n3; }

    cout << "\n Победитель: " << winner << " (" << maxScore << " очков)\n";
    
    return 0;
}