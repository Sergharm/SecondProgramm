#include <iostream>
#include <random>

using namespace std;

int main(){
    int num;
    cin >> num;
    mt19937_64 r;
    uint64_t summa = 0;
    for (int i = 0; i < num; i++){
        uint64_t* x =new uint64_t;
        *x = r();
        summa += *x;
    }
    cout << summa;
    return 0;
}