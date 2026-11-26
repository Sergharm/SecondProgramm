#pragma once
#include <memory>
#include <string>
#include <vector>
#include "linkedList.h"

// АТД «Множество» на хеш-таблице: массив корзин, каждая корзина — односвязный список из ЛР1.
// Добавление, удаление и проверка — O(1) в среднем.
class Set {
public:
    Set();
    bool Add(const std::string& x);      // false, если элемент уже есть
    bool Del(const std::string& x);      // false, если элемента нет
    bool Contains(const std::string& x); // проверка наличия
    int Size() const;
    std::vector<std::string> Items() const;

private:
    std::vector<std::unique_ptr<linkedList>> buckets;
    int count;
    int indexOf(const std::string& x, int n) const;
    void resize(int n);
};
