#pragma once
#include <string>
#include "array.h"

// Задание 6.1, вариант 1: хеш-таблица с открытой адресацией.
// Коллизии разрешаются линейным апробированием (h1 + i) или двойным хешированием (h1 + i*h2).
// Размер — степень двойки, поэтому шаг h2 нечётный и взаимно прост с размером: все ячейки доступны.
// При заполнении на 90 % таблица пересоздаётся вдвое большего размера, все ключи вставляются заново.
class OpenTable {
public:
    OpenTable(int capacity, bool doubleHashing);
    ~OpenTable();
    OpenTable(const OpenTable&) = delete;
    OpenTable& operator=(const OpenTable&) = delete;

    void Put(const std::string& key, const std::string& value);
    bool Get(const std::string& key, std::string& value);
    bool Delete(const std::string& key);
    int Count() const;

    long long Probes;  // сколько ячеек проверено (для анализа)
    int Rehashes;      // сколько раз таблица перестраивалась

private:
    Array* slots;      // ячейки: "" — пусто, DELETED — удалено, иначе ключ + разделитель + значение
    int cap;
    int count;
    bool dbl;

    int step(const std::string& key) const;
    int pos(const std::string& key, int i) const;
    bool find(const std::string& key, int& p);
    void insertNew(const std::string& entry);
    void rehash(int newCap);
};
