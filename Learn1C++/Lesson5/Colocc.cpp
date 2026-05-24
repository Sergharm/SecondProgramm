#include <iostream>
#include <vector>

using namespace std;

ostream& operator<<(ostream& os, const vector<int>& aboba) {
    for (int i : aboba) {
        os << i << " ";
    }
    return os;
}

int main() {
    vector<int> vec = {1, 2, 3, 4, 5};
    cout << vec << endl;  
    return 0;
}