#include <iostream>
using namespace std;

int main() {
    int min, now;
    cin >> now;
    min = now;
    while (now != 0){
        if (now < min){
            min = now;
        }
        cin >> now;
    }
    cout << min;
    return 0;
}