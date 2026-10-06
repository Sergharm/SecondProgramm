// вывод вектора
#include <iostream>
#include <cmath>
#include <vector> 

using namespace std;

vector<int> VectortoString(vector<int>& M1){
    int value;
    while (cin >> value) {
      M1.push_back(value);
    }
    return M1;
}
int main(){
    vector <int> a;
    VectortoString(a);
    for (int value: a){
        cout << value << " ";
    }
    return 0;
}