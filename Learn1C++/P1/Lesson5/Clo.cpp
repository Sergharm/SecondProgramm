#include <iostream>
#include <vector>

using namespace std;

// Перегрузка оператора + (объединение двух векторов)
vector<int> operator+(const vector<int>& left, const vector<int>& right) {
    vector<int> result = left;  // Копируем левый вектор
    result.insert(result.end(), right.begin(), right.end());  // Добавляем правый
    return result;
}

// Альтернатива: перегрузка через дружественную функцию (если внутри класса)
// Но для вектора из STL так делать нельзя, только глобально