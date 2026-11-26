#include "set.h"
#include <functional>
#include <limits>
#include <utility>

Set::Set() : Set(8) {}

Set::Set(size_t initialBuckets) : count(0) {
    if (initialBuckets == 0) initialBuckets = 8;
    buckets.resize(initialBuckets);
    for (auto& b : buckets) {
        b = std::make_unique<linkedList>();
    }
}

size_t Set::BucketIndex(const std::string& elem, size_t bucketCount) const {
    if (bucketCount == 0) return 0;
    return std::hash<std::string>{}(elem) % bucketCount;
}

bool Set::Contains(const std::string& x) const {
    if (buckets.empty()) return false;
    return buckets[BucketIndex(x, buckets.size())]->Find(x) != -1;
}

bool Set::Add(const std::string& x) {
    if (Contains(x)) return false;

    buckets[BucketIndex(x, buckets.size())]->pushFront(x);
    count++;

    // Исключаем переполнения size_t
    size_t cap = buckets.size();
    if (count > cap - cap / 4) {
        resize();
    }
    return true;
}

bool Set::Del(const std::string& x) {
    if (buckets.empty()) return false;
    linkedList* b = buckets[BucketIndex(x, buckets.size())].get();
    if (b->Find(x) == -1) return false;
    b->DelVal(x);
    count--;
    return true;
}

void Set::resize() {
    size_t cap = buckets.size();
    // Защита от переполнения size_t при удвоении размера
    if (cap > std::numeric_limits<size_t>::max() / 2) return;

    // Создание временного множества В X2
    Set expanded(cap * 2);

    // Перенос элементов по index хайв левел
    for (auto& oldBucket : buckets) {
        if (!oldBucket) continue;
        std::string val;
        while (oldBucket->popFront(val)) {
            size_t index = BucketIndex(val, expanded.buckets.size());
            expanded.buckets[index]->pushFront(val);
        }
    }

    // Обмен старых на новые. Trade O(1)
    buckets.swap(expanded.buckets);
}

size_t Set::Size() const {
    return count;
}

std::vector<std::string> Set::Items() const {
    std::vector<std::string> res;
    res.reserve(count);
    for (const auto& b : buckets) {
        if (!b) continue;
        for (const auto& item : b->ToVector()) {
            res.push_back(item);
        }
    }
    return res;
}
