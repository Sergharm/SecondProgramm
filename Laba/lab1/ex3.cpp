#include <iostream>
#include <string>
#include <map>
#include <cctype>
using namespace std;

int romanToInt(string s) {
    map<char, int> roman = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500}, {'M', 1000}
    };
    
    int result = 0, n = s.length();
    for (int i = 0; i < n; i++) {
        if (i < n - 1 && roman[s[i]] < roman[s[i+1]]){
            result -= roman[s[i]];
        }
        else{
            result += roman[s[i]];
        }
    }
    return result;
}

int main() {
    string roman;
    
    cout << "Введите римское число: ";
    cin >> roman;
    while (roman.find_first_not_of("IVXLCDMivxlcdm") != string::npos) {
        cout << "Ошибка! Только римские цифры (I,V,X,L,C,D,M). Повтор: ";
        cin >> roman;
    }
    
    for (char &c : roman) c = toupper(c);
    
    cout << "Результат: " << romanToInt(roman) << endl;
    return 0;
}