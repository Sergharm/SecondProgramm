#include <iostream>

using namespace std; 

ostream& operator(ostream& os, vector<int> aboba){
    for(int i : aboba){
         os << i << " "; 
    }
    return os;
}