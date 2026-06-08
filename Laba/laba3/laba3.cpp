// Программа генерации простых чисел по ГОСТ
// Вариант 3, задание 2

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<int> get_primes(int limit) {
    vector<bool> is_prime(limit + 1, true);
    is_prime[0] = false;
    is_prime[1] = false;
    
    for (int i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    vector<int> result;
    for (int i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            result.push_back(i);
        }
    }
    return result;
}

uint64_t mod_pow(uint64_t a, uint64_t power, uint64_t mod) {
    uint64_t res = 1;
    a = a % mod;
    
    while (power > 0) {

        if (power % 2 == 1) {
            res = (res * a) % mod;
        }
        a = (a * a) % mod;
        power = power / 2;
    }
    return res;
}

bool check_prime_gost(uint64_t p, uint64_t exp) {
    // Проверяем два условия из ГОСТ
    if (mod_pow(2, p - 1, p) == 1) {
        if (mod_pow(2, exp, p) != 1) {
            return true;  
        }
    }
    return false; 
}

// Генерация пары простых чисел (q, p)
// t - нужная разрядность числа p
pair<uint64_t, uint64_t> generate_primes(int t, vector<int> primes) {
    // Вычисляем разрядность для маленького простого q
    int k = (t + 1) / 2;
    
    // Находим подходящие кандидаты для q
    vector<int> q_candidates;
    for (int i = 0; i < primes.size(); i++) {
        int candidate = primes[i];
        // Проверяем диапазон [2^(k-1), 2^k)
        if (candidate >= (1 << (k - 1)) && candidate < (1 << k)) {
            q_candidates.push_back(candidate);
        }
    }
    
    // Основной цикл подбора
    while (true) {
        // Берём случайное q из кандидатов
        int idx = rand() % q_candidates.size();
        uint64_t q = q_candidates[idx];
        
        // Генерируем случайное число xi от 0 до 1
        double xi = (double)rand() / RAND_MAX;
        
        // Считаем N по формуле из ГОСТ
        uint64_t boundary = (uint64_t)1 << (t - 1);  // 2^(t-1)
        uint64_t N = (boundary + q - 1) / q;
        N = N + (uint64_t)((boundary * xi + q - 1) / q);
        
        // Делаем N чётным
        if (N % 2 != 0) {
            N = N + 1;
        }
        
        // Перебираем кандидаты p
        for (uint64_t u = 0; ; u = u + 2) {
            uint64_t p = (N + u) * q + 1;
            
            // Если вышли за разрядность - пробуем другое q
            if (p >= ((uint64_t)1 << t)) {
                break;
            }
            
            // Проверяем на простоту
            if (check_prime_gost(p, N + u)) {
                // Нашли подходящую пару
                pair<uint64_t, uint64_t> ans;
                ans.first = q;
                ans.second = p;
                return ans;
            }
        }
    }
}

int main() {
    // Инициализируем генератор случайных чисел
    srand(time(0));
    
    cout << "=== Генерация простых чисел по ГОСТ ===" << endl;
    
    // Шаг 1: строим таблицу простых чисел
    cout << "Генерирую простые числа до 5000..." << endl;
    vector<int> prime_table = get_primes(5000);
    
    // Выводим для проверки простые до 500
    cout << "Простые числа до 500:" << endl;
    int count = 0;
    for (int i = 0; i < prime_table.size(); i++) {
        if (prime_table[i] > 500) break;
        cout << prime_table[i] << " ";
        count++;
        if (count % 10 == 0) {
            cout << endl;
        }
    }
    cout << endl << endl;
    
    // Ввод разрядности от пользователя
    int bits;
    cout << "Введите разрядность числа p: ";
    cin >> bits;
    
    // Простая проверка ввода
    if (bits < 0 || bits > 30) {
        cout << "Ошибка: разрядность должна быть от 0 до 30" << endl;
        return 1;
    }
    
    // Заголовок таблицы результатов
    cout << "-------------------------------------------------------" << endl;
    cout << "|  №  |      q      |  разрядность  |        p        |" << endl;
    cout << "-------------------------------------------------------" << endl;
    
    // Генерируем 10 пар чисел
    for (int i = 1; i <= 10; i++) {
        pair<uint64_t, uint64_t> res;
        res = generate_primes(bits, prime_table);
        
        // Выводим результат с форматированием
        cout << "| ";
        cout << setw(3) << i << " | ";
        cout << setw(11) << res.first << " | ";
        cout << setw(13) << bits << " | ";
        cout << setw(15) << res.second << " |" << endl;
    }
    
    cout << "-------------------------------------------------------" << endl;
    cout << "Готово! Сгенерировано 10 пар простых чисел." << endl;
    
    return 0;
}