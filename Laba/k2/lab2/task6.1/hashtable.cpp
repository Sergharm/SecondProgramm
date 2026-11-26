#include "hashtable.h"
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

static const char SEP = '\x1f';              // разделитель ключа и значения
static const std::string DELETED(1, '\0');   // надгробие удалённой ячейки

static uint32_t polyHash(const std::string& key) {
    uint32_t h = 0;
    for (unsigned char c : key) {
        h = h * 31u + c;
    }
    return h;
}

static uint32_t altHash(const std::string& key) {
    uint32_t h = 7;
    for (unsigned char c : key) {
        h = h * 131u + c;
    }
    return h;
}

static std::string join(const std::string& k, const std::string& v) {
    return k + SEP + v;
}

static void split(const std::string& e, std::string& k, std::string& v) {
    size_t i = e.find(SEP);
    if (i == std::string::npos) {
        k = e;
        v.clear();
        return;
    }
    k = e.substr(0, i);
    v = e.substr(i + 1);
}

static Array* newSlots(int n) {
    Array* a = new Array();
    for (int i = 0; i < n; i++) a->PushBack("");
    return a;
}

OpenTable::OpenTable(int capacity, bool doubleHashing)
    : Probes(0), Rehashes(0), slots(nullptr), cap(2), count(0), dbl(doubleHashing) {
    const int MAX_CAP = 1 << 30;
    if (capacity > MAX_CAP) capacity = MAX_CAP;
    while (cap < capacity && cap <= MAX_CAP / 2) {
        cap *= 2;
    }
    slots = newSlots(cap);
}

OpenTable::~OpenTable() {
    delete slots;
}

int OpenTable::step(const std::string& key) const {
    if (!dbl || cap <= 2) return 1;
    uint32_t half = static_cast<uint32_t>(cap) / 2u;
    return 1 + 2 * static_cast<int>(altHash(key) % half); // нечётный шаг от 1 до cap-1
}

int OpenTable::pos(const std::string& key, int i) const {
    uint64_t h1 = polyHash(key);
    uint64_t offset = static_cast<uint64_t>(i) * static_cast<uint64_t>(step(key));
    return static_cast<int>((h1 + offset) % static_cast<uint32_t>(cap));
}

bool OpenTable::find(const std::string& key, int& p) {
    for (int i = 0; i < cap; i++) {
        p = pos(key, i);
        Probes++;
        std::string s = slots->At(p);
        if (s.empty()) return false;          // дальше ключа точно нет
        if (s == DELETED) continue;
        std::string k, v;
        split(s, k, v);
        if (k == key) return true;
    }
    return false;
}

bool OpenTable::Get(const std::string& key, std::string& value) {
    int p;
    if (!find(key, p)) return false;
    std::string k;
    split(slots->At(p), k, value);
    return true;
}

void OpenTable::Put(const std::string& key, const std::string& value) {
    int p;
    if (find(key, p)) {
        slots->Set(p, join(key, value));
        return;
    }
    // Проверка загрузки > 90% в беззнаковом uint64_t (без переполнения int)
    if (static_cast<uint64_t>(count + 1) * 10u > static_cast<uint64_t>(cap) * 9u) {
        const int MAX_CAP = 1 << 30;
        if (cap <= MAX_CAP / 2) {
            rehash(cap * 2);
        } else if (count >= cap) {
            return;
        }
    }
    insertNew(join(key, value));
    count++;
}

void OpenTable::insertNew(const std::string& entry) {
    std::string key, value;
    split(entry, key, value);
    for (int i = 0; i < cap; i++) {
        int p = pos(key, i);
        Probes++;
        std::string s = slots->At(p);
        if (s.empty() || s == DELETED) {
            slots->Set(p, entry);
            return;
        }
    }
}

bool OpenTable::Delete(const std::string& key) {
    int p;
    if (!find(key, p)) return false;
    slots->Set(p, DELETED);
    count--;
    return true;
}

int OpenTable::Count() const {
    return count;
}

void OpenTable::rehash(int newCap) {
    if (newCap <= 0) return;
    Rehashes++;
    std::vector<std::string> live;
    live.reserve(static_cast<size_t>(count));
    for (int i = 0; i < slots->Size(); i++) {
        std::string s = slots->At(i);
        if (!s.empty() && s != DELETED) live.push_back(std::move(s));
    }
    delete slots;
    cap = newCap;
    slots = newSlots(cap);
    for (const auto& e : live) insertNew(e);
}
