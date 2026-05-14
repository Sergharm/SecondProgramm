#include<iostream>
#include<vector>

using namespace std;

int mod(int a, int m) {
    return (a % m + m) % m;
}

vector<int> FibLag(int a, int b, char op, int m, const vector<int>& init, int count) {
    vector<int> seq = init;
    vector<int> result;  
    int maxLag = max(a, b);
    
    for (int n = maxLag; n < maxLag + count; ++n) { 
        int val;
        switch(op) {
            case '+': 
                val = seq[n - a] + seq[n - b]; 
                break;
            case '-': 
                val = seq[n - a] - seq[n - b]; 
                break;
            case '*': 
                val = seq[n - a] * seq[n - b]; 
                break;
            case '^': 
                val = seq[n - a] ^ seq[n - b]; 
                break;
            default: 
                val = 0;
        }
        int resultVal = mod(val, m);
        seq.push_back(resultVal);
        result.push_back(resultVal);  
    }
    
    return result;
}

int main() {
    int a, b, m, count;
    char op;
    
    cin >> a >> b >> op >> m >> count;
    
    vector<int> initial(max(a, b));
    for (int& val : initial) {
        cin >> val;
    }
    
    vector<int> sequence = FibLag(a, b, op, m, initial, count);
    
    for (int val : sequence) {
        cout << val << " ";
    }
    cout << endl;
    
    return 0;
}