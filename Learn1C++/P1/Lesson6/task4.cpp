#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <clocale>

using namespace std;

struct Athlete {
    int id;
    vector<int> scores;
};

int getTotalScore(const Athlete& a) {
    int sum = 0;
    for (int score : a.scores) {
        sum += score;
    }
    return sum;
}

int getMaxScore(const Athlete& a) {
    int max_score = 0;
    for (int score : a.scores) {
        if (score > max_score) {
            max_score = score;
        }
    }
    return max_score;
}

bool compareAthletes(const Athlete& a, const Athlete& b) {
    int sumA = getTotalScore(a);
    int sumB = getTotalScore(b);

    if (sumA != sumB) {
        return sumA > sumB;
    }
    
    int maxA = getMaxScore(a);
    int maxB = getMaxScore(b);
    if (maxA != maxB) {
        return maxA > maxB;
    }
    
    return a.id < b.id;
}

map<int, int> rankAthletes(vector<Athlete> athletes) {
    sort(athletes.begin(), athletes.end(), compareAthletes);
    map<int, int> ranking;
    for (int i = 0; i < athletes.size(); i++) {
        ranking[athletes[i].id] = i + 1;  // Ранги начинаются с 1
    }
    
    return ranking;
}

// Пример использования
int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    vector<Athlete> athletes = {
        {1, {10, 9, 8}},      // сумма: 27, макс: 10
        {2, {10, 10, 7}},     // сумма: 27, макс: 10
        {3, {9, 9, 9}},       // сумма: 27, макс: 9
        {4, {10, 10, 10}}     // сумма: 30, макс: 10
    };
    
    map<int, int> result = rankAthletes(athletes);
    
    cout << "Ранжирование:" << endl;
    for (auto& pair : result) {
        cout << "Атлет " << pair.first << " -> Ранг " << pair.second << endl;
    }
    
    return 0;
}