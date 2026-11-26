#pragma once
#include <string>
#include "doublyList.h"
#include "hashtable.h"

// Задание 7, вариант 1: кэш LRU (LeastRecentlyUsed).
// Значения — в хеш-таблице из задания 6.1; порядок обращений — в двусвязном списке из ЛР1:
// голова — самый давний ключ, хвост — самый свежий.
class LRU {
public:
    explicit LRU(int capacity);
    void Set(const std::string& key, const std::string& value);
    std::string Get(const std::string& key); // значение или "-1"

private:
    int cap;
    OpenTable table;
    DoublyList order;
    void touch(const std::string& key); // переносит ключ в хвост
};
