#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Array {
private:
    string* data = nullptr;
    int isUsed = 0; // записанно элементов
    int highlighted = 10; // под сколько элементов выделенно
    void clear();
    void increaseArrasy(); // увеличиваем если переполнен
public:
    ~Array();
    Array();
    void Create();
    void Print();
    void PushBack(const string& value);
    void Insert(const int& indx, const string& value); // добавление по индексу 
    void Get(const int& indx); // получение по индексу
    void DelIndx(const int& indx); // удаление по индексу 
    void Length(); // длина массива
    void Set(int indx, string value); // замена  
    int Size(); // количество элементов (без вывода)
    string At(const int& indx); // вернуть элемент по индексу (без вывода)
    string ToString(); // все элементы через пробел — для записи в файл
};
