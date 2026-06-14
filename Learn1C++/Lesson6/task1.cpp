#include <iostream>

using namespace std;


int main(){
    double **p = 0;
    p = new double*;
    *p = new double(2);
    cout << **p << endl;
    delete *p;
    delete p;
    p = 0;


    return 0; 
}