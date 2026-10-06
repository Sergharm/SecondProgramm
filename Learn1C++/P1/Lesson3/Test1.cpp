// Игра
#include <map>
#include <iostream>
#include <string>
using namespace std;

int SumTovarsAll(map<string, int> M2){
    int sum = 0;
    for(pair<string, int> value: M2){
        sum += value.second;
    }
    cout << sum << endl;
    return sum;
}

map<string, int> DeficiteTovars(map<string, int> M3){
    for(pair<string, int> value1: M3){
        if (value1.second < 15){
            cout << "Дефицит товаров:" << " " << value1.first << " " << 15 - value1.second << "\n";
        }
        else{
            cout << value1.first << ": " << "хватает, закупать не нужно!!!(Роман доволен)" << "\n";
        }
    }
    return M3;
}
int main(){
    int sum = 0;
    map<string, int> M1 {{"carrot", 15}, {"apples", 10}, {"tomato", 35}, {"banana", 5}, {"minions", 201}};
    sum = SumTovarsAll(M1);
    DeficiteTovars(M1);
    return 0;
}



//Каждый 15 штук минимум, если меньше надо заказать