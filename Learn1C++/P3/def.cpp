#include <iostream>
using namespace std;

class Vector {
public:
    int* data; 
    size_t sz;

    Vector(int a) : data(new int[a]()), sz(a) {
       
    }


    Vector(int b, int c) {
        data = new int[b]; 
        sz = b;
        for (int i = 0; i < b; i++) {
            data[i] = c;
        }
    }

    ~Vector() {
        delete[] data; 
    }

    void Print() {
        for (size_t i = 0; i < sz; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    int Sum() {
        int s = 0;
        for (size_t i = 0; i < sz; ++i) {
            s += data[i];
        }
        return s;
    }

    int Max() {
        if (sz == 0){
            return 0;
        } 
        int max = data[0];
        for (size_t i = 1; i < sz; ++i) {
            if (data[i] > max) {
                max = data[i];
            }
        }
        return max;
    }

    int Min() {
        if (sz == 0){
            return 0;
        }
        int min = data[0];
        for (size_t i = 1; i < sz; ++i) {
            if (data[i] < min) {
                min = data[i];
            }
        }
        return min;
    }

    int operator[] (int l){

        
    }


    Vector operator+ (const Vector l){
        for (int i = 0; i <= sz; i++){
            Vector sum(sz, 0);
            for (int i = 0; i <= sz; ++i){
                sum.data[i] =l.data[i] + data[i];
            }
            return sum;
        }

    }
};

int main() {
    Vector v(20);     
    Vector v1(30, 1);
    v1.Print();
    int x = v1.Sum();
    cout << x << endl;
    int min = v.Min();
    cout << min << endl;
    int max = v.Max();
    cout << max << endl;

    return 0;
}
