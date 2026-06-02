#include <iostream>
#include <cstdint>
#include <exception>

using namespace std;

int rating(int x, int y){
    if (x == 0 && y == 0){
        throw "negodyai";
    }
    return (x + y) / 2;
}

int main(){

    int x = 0; // семестр
    int y = 0; // экзамен
    try{
    cout << rating(x, y) << endl;
    } catch(const char* str) {
        cerr << str << endl;
    }
    return 0;
}