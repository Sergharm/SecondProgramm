#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

int longestOnes(string s, int K) {
    int left = 0;          
    int zeroCount = 0;     
    int maxLen = 0;       
    
    for (int right = 0; right < s.length(); right++) {
        if (s[right] == '0') {
            zeroCount++;
        }

        while (zeroCount > K) {
            if (s[left] == '0') {
                zeroCount--;
            }
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}



int main() {
    string s;
    int K;
    bool valid = false;
    while (!valid) {
        cout << "Введите строку из 0 и 1: ";
        cin >> s;
        valid = true;  
        for (char val : s) {
            if (val != '0' && val != '1') {
            cout << "Вам нужно ввести строку из 0 и 1. Попробуйте еще раз!" << endl;
            valid = false;  
            break;
            }
        }
    }
    cout << "Введите K (сколько нулей можно заменить): ";
    cin >> K;
    
 
    while (cin.fail()) {
        cin.clear();  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // очищаем буфер
        cout << "Ошибка! Нужно ввести ЧИСЛО. Попробуйте ещё раз: ";
        cin >> K;
    }
     cout << longestOnes(s, K) << endl;
}