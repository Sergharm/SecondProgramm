// перевернуть мап
#include <map>
#include <iostream>
#include <string>

using namespace std;

map<int, string> mapFlip(map<string, int> M2){
    map<int, string> bimbo;
    for (pair <string, int> val: M2){
        bimbo[val.second] = val.first; // bimbo.emplace(val.second, val.first)
    }
    return bimbo;
}


void PrintMap(map<int, string> M3){
    for (pair<int, string> val1: M3){
        cout << val1.first << " " << val1.second << endl;
    }
}

int main(){
    map<string, int> M1;
    int value;
    string str;
    while(cin >> str >> value){
        M1.emplace(str, value);
    }
    PrintMap(mapFlip(M1));
}
