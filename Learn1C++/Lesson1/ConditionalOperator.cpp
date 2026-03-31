#include <iostream>
using namespace std;

int main(){
    int eyes, legs;
    cin >> eyes >> legs;
    if ( eyes >= 8){
        if (legs == 8){
            cout << "spider";
        }
        else{
            cout << "scallop";
        }
    }
    else{
        if (legs == 4 && eyes == 2){
            cout << "cat";
        }
        else{
            cout << "bug";
        }
    }
    return 0;
}