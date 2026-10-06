#include <iostream>
#include <vector>

using namespace std;

ostream& operator<<(ostream& so, const vector<int>& aboba){
    for (int i: aboba){
        so << i << " ";
        return so;
    }
}
int main() {
    vector<int> vec = {1, 2, 3, 4, 5};
    cout << vec << endl;  
    return 0;
}
