#include <iostream>
#include <vector>
#include <clocale>
using namespace std;


vector<pair<char, int>> packSymbols(const vector<char>& data){
    vector<pair<char, int>> result;
    if (data.empty()) {
        return result;
    }
    result.push_back({data[0], 1});
      for (size_t i = 1; i < data.size(); ++i) {
        // Если текущий символ совпадает с предыдущим
        if (data[i] == result.back().first) {
            result.back().second++; // Увеличиваем счетчик
        } else {
            result.push_back({data[i], 1}); // Создаем новую пару
        }
    }
    
    return result;
}

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    vector <char> data = {'a', 'a', 'b', 'c', 'c', 'c', 'a', 'd', 'd'};
    vector<pair<char, int>> result = packSymbols(data);
    cout << "Результат: {";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "{'" << result[i].first << "', " << result[i].second << "}";
        if (i < result.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
    
    return 0;
}