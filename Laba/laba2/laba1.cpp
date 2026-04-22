#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double gr(double x) {
    if (x < -2) {
        return -2 * x - 5;
    }
    else if (x >= -2 && x < 0) {
        return -1 - sqrt(1 - pow(x + 1, 2));
    }
    else if (x >= 0 && x < 2) {
        return 2 - sqrt(4 - pow(x, 2));
    }
    else if (x >= 2 && x < 4) {
        return -1;
    }
    else {
        return 0.5 * x - 3;
    }
}

int main() {
    double xStart, xEnd, dx;
    
    cout << "Enter start, end, step: ";
    cin >> xStart >> xEnd >> dx;
    
    cout << fixed << setprecision(2);
    cout << "|     x      |    f(x)    |" << endl;
    cout << "|------------|------------|" << endl;
    
    for (double x = xStart; x <= xEnd + 0.001; x += dx) {
        cout << "| " << setw(10) << x 
             << " | " << setw(10) << gr(x) << " |" << endl;
    }
    
    return 0;
}