#include <iostream>
using namespace std;

int main() {
    int summ, now;
    cin >> now;
    summ = 0;
    while (now != 0){
            summ = summ + now;
            cin >> now;
        }
    cout << summ;
    return 0;
}