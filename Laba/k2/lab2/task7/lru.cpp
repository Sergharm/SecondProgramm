#include "lru.h"

LRU::LRU(int capacity) : cap(capacity), table(16, true) {}

void LRU::touch(const std::string& key) {
    order.DelVal(key);   // поиск ключа линейный: O(cap)
    order.pushBack(key);
}

void LRU::Set(const std::string& key, const std::string& value) {
    if (cap <= 0) return;
    std::string tmp;
    if (table.Get(key, tmp)) {
        table.Put(key, value);
        touch(key);
        return;
    }
    if (table.Count() >= cap) {
        std::string oldest = order.At(0); // самый давний
        order.delFront();
        table.Delete(oldest);
    }
    table.Put(key, value);
    order.pushBack(key);
}

std::string LRU::Get(const std::string& key) {
    std::string value;
    if (!table.Get(key, value)) return "-1";
    touch(key);
    return value;
}
