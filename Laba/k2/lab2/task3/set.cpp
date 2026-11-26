#include "set.h"
#include <sstream>

// полиномиальная хеш-функция: h = h*31 + код символа
static int polyHash(const std::string& key) {
    long long h = 0;
    for (unsigned char c : key) {
        h = (h * 31 + c) & 0x7fffffff;
    }
    return (int)h;
}

Set::Set() : count(0) {
    buckets.resize(8);
    for (auto& b : buckets) b.reset(new linkedList());
}

int Set::indexOf(const std::string& x, int n) const {
    return polyHash(x) % n;
}

bool Set::Contains(const std::string& x) {
    return buckets[indexOf(x, (int)buckets.size())]->Find(x) != -1;
}

bool Set::Add(const std::string& x) {
    if (Contains(x)) return false;
    buckets[indexOf(x, (int)buckets.size())]->pushFront(x); // новый элемент — в голову
    count++;
    if (count * 4 > (int)buckets.size() * 3) { // заполнено больше 75 %
        resize((int)buckets.size() * 2);
    }
    return true;
}

bool Set::Del(const std::string& x) {
    linkedList* b = buckets[indexOf(x, (int)buckets.size())].get();
    if (b->Find(x) == -1) return false;
    b->DelVal(x);
    count--;
    return true;
}

void Set::resize(int n) {
    std::vector<std::string> items = Items();
    buckets.clear();
    buckets.resize(n);
    for (auto& b : buckets) b.reset(new linkedList());
    for (const auto& x : items) {
        buckets[indexOf(x, n)]->pushFront(x);
    }
}

int Set::Size() const {
    return count;
}

std::vector<std::string> Set::Items() const {
    std::vector<std::string> res;
    for (const auto& b : buckets) {
        std::istringstream is(b->ToString()); // элементы в корзине разделены пробелом
        std::string w;
        while (is >> w) res.push_back(w);
    }
    return res;
}
