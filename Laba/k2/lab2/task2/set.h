#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "linkedList.h"

// АТД «Множество» на хеш-таблице: массив корзин, каждая корзина — односвязный список.
// Добавление, удаление и проверка — O(1) в среднем.
class Set {
public:
    Set();
    explicit Set(size_t initialBuckets);

    bool Add(const std::string& x);            // false, если элемент уже есть
    bool Del(const std::string& x);            // false, если элемента нет
    bool Contains(const std::string& x) const; // проверка наличия
    size_t Size() const;
    std::vector<std::string> Items() const;

private:
    std::vector<std::unique_ptr<linkedList>> buckets;
    size_t count;

    size_t BucketIndex(const std::string& elem, size_t bucketCount) const;
    void resize();
};
