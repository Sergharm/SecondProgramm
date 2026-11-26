#include "hashtable.h"
#include <vector>

static const char SEP = '\x1f';              // разделитель ключа и значения
static const std::string DELETED(1, '\0');   // надгробие удалённой ячейки

static int polyHash(const std::string& key) {
    long long h = 0;
    for (unsigned char c : key) h = (h * 31 + c) & 0x7fffffff;
    return (int)h;
}

static int altHash(const std::string& key) {
    long long h = 7;
    for (unsigned char c : key) h = (h * 131 + c) & 0x7fffffff;
    return (int)h;
}

static std::string join(const std::string& k, const std::string& v) {
    return k + SEP + v;
}

static void split(const std::string& e, std::string& k, std::string& v) {
    size_t i = e.find(SEP);
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
    while (cap < capacity) cap *= 2;
    slots = newSlots(cap);
}

OpenTable::~OpenTable() {
    delete slots;
}

int OpenTable::step(const std::string& key) const {
    if (!dbl) return 1;
    return 1 + 2 * (altHash(key) % (cap / 2)); // нечётный шаг от 1 до cap-1
}

int OpenTable::pos(const std::string& key, int i) const {
    return (int)((polyHash(key) + (long long)i * step(key)) % cap);
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
    if ((count + 1) * 10 > cap * 9) { // заполнено больше 90 %
        rehash(cap * 2);
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
    Rehashes++;
    std::vector<std::string> live;
    for (int i = 0; i < slots->Size(); i++) {
        std::string s = slots->At(i);
        if (!s.empty() && s != DELETED) live.push_back(s);
    }
    delete slots;
    cap = newCap;
    slots = newSlots(cap);
    for (const auto& e : live) insertNew(e);
}
