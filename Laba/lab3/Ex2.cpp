#include <iostream>
#include <vector>
using namespace std;

int main() {
    // Считываем количество чисел и максимальное количество для взятия
    int n, m;
    cin >> n >> m;
    
    // Считываем массив чисел
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    // Вычисляем префиксные суммы для быстрого подсчёта сумм на отрезках
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + a[i];
    }
    
    // Счета игроков и позиция в массиве
    int pavelScore = 0;
    int vikaScore = 0;
    int pos = 0;
    
    // Сколько чисел взял каждый игрок в свой предыдущий ход
    // -1 означает, что хода ещё не было
    int lastPavel = -1;
    int lastVika = -1;
    
    // Чей ход: 0 - Павел, 1 - Вика
    int turn = 0;
    
    // Играем пока не кончатся числа
    while (pos < n) {
        // Максимальное количество чисел, которое можно взять
        int maxTake = min(m, n - pos);
        
        // Запрещённое количество (сколько нельзя брать)
        int forbidden = -1;
        if (turn == 0) {
            forbidden = lastPavel;  // Павел не может взять столько, сколько взял в прошлый раз
        } else {
            forbidden = lastVika;   // Вика не может взять столько, сколько взяла в прошлый раз
        }
        
        // Ищем лучший ход
        int bestSum = -1000000000;  // Очень маленькое число
        int bestTake = 1;
        
        // Пробуем взять от 1 до maxTake чисел
        for (int take = 1; take <= maxTake; take++) {
            // Пропускаем запрещённое количество
            if (take == forbidden) {
                continue;
            }
            
            // Считаем сумму take чисел начиная с позиции pos
            int currentSum = prefix[pos + take] - prefix[pos];
            
            // Если сумма больше, или сумма такая же, но берём меньше чисел
            if (currentSum > bestSum || (currentSum == bestSum && take < bestTake)) {
                bestSum = currentSum;
                bestTake = take;
            }
        }
        
        // Добавляем сумму к счёту текущего игрока
        if (turn == 0) {
            pavelScore += bestSum;
            lastPavel = bestTake;  // Запоминаем, сколько взял Павел
        } else {
            vikaScore += bestSum;
            lastVika = bestTake;   // Запоминаем, сколько взяла Вика
        }
        
        // Переходим к следующей позиции
        pos += bestTake;
        
        // Меняем игрока
        turn = 1 - turn;
    }
    
    // Определяем победителя
    if (pavelScore > vikaScore) {
        cout << 1 << endl;
    } else {
        cout << 0 << endl;
    }
    
    return 0;
}