#pragma once
#include <iostream>
#include <string>
#include <vector>

struct linkedList {
private:
    struct Node {
        std::string data;
        Node* next;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    void clear();

public:
    linkedList() = default;
    ~linkedList();

    // Запрет копирования для защиты от двойного освобождения памяти
    linkedList(const linkedList&) = delete;
    linkedList& operator=(const linkedList&) = delete;

    void pushFront(const std::string& value);
    void pushBack(const std::string& value);
    void pushBefore(const std::string& tar, const std::string& value); // добавить элемент до
    void pushAfter(const std::string& tar, const std::string& value);  // добавить элемент после

    void GetHead() const; // прочитать голову
    void GetTail() const; // прочитать хвост
    void GetIndx(const int& indx) const; // прочитать по indx

    void DelVal(const std::string& value); // удалить по значению
    void delFront(); // удалить голову
    void delBack();  // удалить хвост
    void delBefore(const std::string& tar); // удалить элемент перед tar
    void delAfter(const std::string& tar);  // удалить элемент после tar
    bool popFront(std::string& out);        // извлечь элемент из головы (для быстрого переноса в Set)

    void GetVal(const std::string& value) const; // поиск элемента по значению
    void Print() const;
    int Size() const; // количество элементов
    std::string At(const int& indx) const; // вернуть элемент по индексу
    int Find(const std::string& value) const; // вернуть индекс, -1 если не найден
    std::string ToString() const; // все элементы через пробел для записи в файл
    std::vector<std::string> ToVector() const; // получить все элементы вектором без потери пробелов
};
