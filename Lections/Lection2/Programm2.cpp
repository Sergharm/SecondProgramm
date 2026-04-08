#include <iostream>

using namespace std;
void ff(int f){
    cout << f << f << endl;
    f++;
    ff(f);
}
int main(){
    ff(0);
    return 0;   
}