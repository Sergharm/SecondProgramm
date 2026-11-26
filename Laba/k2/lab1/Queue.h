#pragma once
#include <iostream>
#include <string>

struct Queue {
private:
    struct Node {
        std::string value;
        Node* next = nullptr;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    void clear();
public:
    ~Queue();
    void push(const std::string& value); // добавить в конец
    void pop(); // удалить из начала
    void peek(); // прочитать первый элемент
    void Print();
    int Size(); // количество элементов
    std::string ToString(); // элементы через пробел от начала к концу для записи в файл
};