#include <iostream>
#include <map>
#include <vector>

using namespace std;

// Банковская система. Лол
map<int, string> mapFlip(map<string, int> M2){  // функция для переворота ключа и значения
    map<int, string> bimbo;
    for (pair <string, int> val: M2){
        bimbo[val.second] = val.first; 
    }
    return bimbo;
}

void PrintMap(map<int, string> M3){         // Вывод map в консоль
    for (pair<int, string> val1: M3){
        cout << val1.first << " " << val1.second << endl;
    }
}

int main(){         // Основная функция
    map<string, int> M1;
    int value;
    string str;
    while(cin >> str >> value){
        M1.emplace(str, value); 
    }
    PrintMap(mapFlip(M1));   
    return 0;
    
}