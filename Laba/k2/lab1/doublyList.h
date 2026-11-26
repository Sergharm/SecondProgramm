#pragma once 
#include <iostream>
#include <string>

using namespace std;

struct DoublyList {
private:
    struct Node{
        string value;
        Node* next = nullptr;
        Node* prev = nullptr;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    void clear();
public:
    ~DoublyList();
    void pushFront(const string& value); 
    void pushBack(const string& value);
    void pushBefore(const string& tar, const string& value); //  добавить элемент до
    void pushAfter(const string& tar, const string& value); //  добавить элемент после
    void GetHead(); // прочитать голову
    void GetTail(); // прочитать хвост
    void GetIndx(const int& indx); // прочитать по indx
    void DelVal(const string& value); // удалить по значению 
    void delFront(); // удалить голову
    void delBack();  // удалить хвост
    void delBefore(const string& tar); // удалить элемент перед indx
    void delAfter(const string& indx); // удалить элемент после indx
    void GetVal(const string& value); // поиск элемента по значению
    void Print();
    int Size(); // количество элементов
    string At(const int& indx); // вернуть элемент по индексу
    int Find(const string& value); // вернуть индекс, -1 если не найден
    string ToString(); // все элементы через пробел — для записи в файл
};
