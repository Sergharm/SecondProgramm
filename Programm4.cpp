#include <iostream>
#include <random>
#include <cstdint>

using namespace std;

int main(){
    int* array = new int[5];
//    cout << *(array + 1) << "\n";
    for (int i = 0; i < 5; ++i){
        cout << (array + i) << " " << "\n";
    }
    cout << endl << array[3];
    delete []array;
    delete []array;
 //   cout << endl << array[3];
    return 0;
}