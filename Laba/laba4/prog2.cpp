#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <locale>
using namespace std;

void genVec(const int& n, vector<int>& originVec) {
    random_device rd;
    mt19937 gener(rd());
    uniform_int_distribution<> distr(100, 150);
    for (int i = 0; i < n; ++i) {
        originVec.push_back(distr(gener));
    }
}

pair<int, int> findMinMax(const vector<int>& vect) {
    int minIdx = 0, maxIdx = 0;
    for (size_t i = 1; i < vect.size(); ++i) {
        if (vect[i] < vect[minIdx]) {
            minIdx = i;
        }
        if (vect[i] > vect[maxIdx]) {
            maxIdx = i;
        }
    }
    return {minIdx, maxIdx};
}

int findSecondMax(const vector<int>& vect) {
    vector<int> sorted = vect;
    sort(sorted.begin(), sorted.end(), greater<int>());
    for (size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] != sorted[0]) {
            return sorted[i];
        }
    }
    return sorted[0];
}

int sumBetweenMinAndSecondMax(const vector<int>& vect) {
    auto minMax = findMinMax(vect);
    int minIdx = minMax.first;
    int secondMax = findSecondMax(vect);
    
    int secondMaxIdx = -1;
    for (size_t i = 0; i < vect.size(); ++i) {
        if (vect[i] == secondMax && static_cast<int>(i) != minIdx) {
            secondMaxIdx = i;
            break;
        }
    }
    
    int start = min(minIdx, secondMaxIdx) + 1;
    int end = max(minIdx, secondMaxIdx);
    
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += vect[i];
    }
    return sum;
}

void generateArr3(const vector<int>& arr1, const vector<int>& arr2, vector<int>& arr3) {
    for (size_t i = 0; i < arr1.size(); ++i) {
        if (i % 2 == 0) {
            arr3.push_back(arr1[i] + arr2[i]);
        } else {
            arr3.push_back(arr1[i] - arr2[i]);
        }
    }
}

void countUniqueElements(const vector<int>& vect) {
    map<int, int> counts;
    for (int val : vect) {
        counts[val]++;
    }
    
    cout << "Уникальные элементы и их количество:" << endl;
    for (const auto& pair : counts) {
        cout << pair.first << ": " << pair.second << " раз(а)" << endl;
    }
}

bool isLeapYear(int year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return year % 4 == 0;
}

void findLeapYears(int startYear, int endYear, vector<int>& leapYears) {
    for (int year = startYear; year <= endYear; ++year) {
        if (isLeapYear(year)) {
            leapYears.push_back(year);
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int n;
    cout << "Введите размер массива n (n >= 10): ";
    cin >> n;

    if (n < 10) {
        cout << "Размер массива должен быть больше или равен 10" << endl;
        return 0;
    }

    vector<int> originVec;
    genVec(n, originVec);

    cout << "\nЗадание 1" << endl;
    cout << "Исходный массив:" << endl;
    for (size_t i = 0; i < originVec.size(); ++i) {
        cout << originVec[i] << " ";
        if ((i + 1) % 10 == 0) {
            cout << endl;
        }
    }
    cout << endl;

    cout << "Задание 2" << endl;
    int secondMax = findSecondMax(originVec);
    cout << "Второй по величине элемент: " << secondMax << endl;
    
    auto minMax = findMinMax(originVec);
    cout << "Минимальный элемент: " << originVec[minMax.first] 
         << " (индекс " << minMax.first << ")" << endl;
    
    int sum = sumBetweenMinAndSecondMax(originVec);
    cout << "Сумма элементов между минимальным и вторым по величине: " << sum << endl;
    cout << endl;

    cout << "Задание 3" << endl;
    vector<int> arr1, arr2, arr3;
    random_device rd;
    mt19937 gener(rd());
    uniform_int_distribution<> distr(-50, 50);
    
    for (int i = 0; i < n; ++i) {
        arr1.push_back(distr(gener));
        arr2.push_back(distr(gener));
    }
    
    generateArr3(arr1, arr2, arr3);
    
    cout << "arr1: ";
    for (int val : arr1) cout << val << " ";
    cout << endl;
    
    cout << "arr2: ";
    for (int val : arr2) cout << val << " ";
    cout << endl;
    
    cout << "arr3: ";
    for (int val : arr3) cout << val << " ";
    cout << endl;
    cout << endl;

    cout << "Задание 4" << endl;
    countUniqueElements(originVec);
    cout << endl;

    cout << "Задание 5" << endl;
    int startYear, endYear;
    cout << "Введите начальный год: ";
    cin >> startYear;
    cout << "Введите конечный год: ";
    cin >> endYear;
    
    vector<int> leapYears;
    findLeapYears(startYear, endYear, leapYears);
    
    cout << "Високосные года в диапазоне [" << startYear << ", " << endYear << "]: ";
    for (int year : leapYears) {
        cout << year << " ";
    }
    cout << endl;

    return 0;
}