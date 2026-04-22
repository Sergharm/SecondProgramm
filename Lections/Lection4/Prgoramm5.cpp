#include <iostream>

using namespace std;

int mod( int base, int power, int modulo){
    base %= modulo;

    int result = 1;
    for (int i = 0; i < power; ++i){
        result *= base;
        result %= modulo;

    }
    return result;
}

int main(){
    cout << mod(2, 2025, 5) << endl;
    return 0;
}