#include <iostream>
using namespace std;

int main(){
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    if((a == c) && (((b - 1) == d) || ((b + 1 ) == d) || (b == d))){
        cout << "YES";
    }
    else if((b == d) && (((a -1) == c) || ((a + 1) == c) || (a == c))){
        cout << "YES";
    }
    else if ((((a + 1) == c) && (b + 1) == d) || (((a - 1) == c) && ((b + 1) == d)) || ((((a + 1) == c) && ((b - 1) == d))) || ((((a - 1) == c) && (b + 1) == d))){
        cout << "YES";
    }
    else{
        cout << "NO";
    }
    return 0;  
}