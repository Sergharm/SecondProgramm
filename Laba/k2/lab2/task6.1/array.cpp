#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include "array.h"

using namespace std;

Array::~Array() {
    clear();
}

Array::Array() {
    Create();
}

Array::Array(Array&& other) noexcept
    : data(other.data), isUsed(other.isUsed), highlighted(other.highlighted) {
    other.data = nullptr;
    other.isUsed = 0;
    other.highlighted = 0;
}

Array& Array::operator=(Array&& other) noexcept {
    if (this != &other) {
        clear();
        data = other.data;
        isUsed = other.isUsed;
        highlighted = other.highlighted;
        other.data = nullptr;
        other.isUsed = 0;
        other.highlighted = 0;
    }
    return *this;
}

void Array::Create() {
    clear();
    highlighted = 10;
    data = new string[highlighted];
    isUsed = 0;
}

void Array::increaseArrasy() {
    int newCap = (highlighted <= 0) ? 10 : highlighted;

    if (highlighted > 0) {
        if (highlighted > numeric_limits<int>::max() / 2) {
            return; // защита от переполнения int
        }
        newCap = highlighted * 2;
    }

    string* newData = new string[newCap];
    for (int i = 0; i < isUsed; ++i) {
        newData[i] = std::move(data[i]);
    }
    delete[] data;
    data = newData;
    highlighted = newCap;
}

void Array::PushBack(const string& value) {
    if (data == nullptr || isUsed >= highlighted) {
        int oldCap = highlighted;
        increaseArrasy();
        if (data == nullptr || (isUsed >= highlighted && highlighted == oldCap)) {
            return;
        }
    }
    data[isUsed] = value;
    ++isUsed;
}

void Array::Print() const {
    for (int i = 0; i < isUsed; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;
}

void Array::Insert(const int& indx, const string& value) {
    if (indx < 0 || indx > isUsed) {
        return;
    }
    if (data == nullptr || isUsed >= highlighted) {
        int oldCap = highlighted;
        increaseArrasy();
        if (data == nullptr || (isUsed >= highlighted && highlighted == oldCap)) {
            return;
        }
    }
    for (int i = isUsed; i > indx; --i) {
        data[i] = std::move(data[i - 1]);
    }
    data[indx] = value;
    ++isUsed;
}

void Array::clear() {
    delete[] data;
    data = nullptr;
    isUsed = 0;
    highlighted = 0;
}

void Array::Length() const {
    cout << "Длина массива: " << isUsed << endl;
}

void Array::Get(const int& indx) const {
    if (indx < 0 || indx >= isUsed) {
        return;
    }
    cout << data[indx] << endl;
}

void Array::DelIndx(const int& indx) {
    if (indx < 0 || indx >= isUsed) {
        return;
    }
    for (int i = indx; i < isUsed - 1; ++i) {
        data[i] = std::move(data[i + 1]);
    }
    data[isUsed - 1].clear();
    --isUsed;
}

void Array::Set(int indx, string val) {
    if (indx < 0 || indx >= isUsed) {
        return;
    }
    data[indx] = std::move(val);
}

int Array::Size() const {
    return isUsed;
}

string Array::At(const int& indx) const {
    if (indx < 0 || indx >= isUsed) {
        return "";
    }
    return data[indx];
}

string Array::ToString() const {
    string res;
    for (int i = 0; i < isUsed; ++i) {
        if (i > 0) res += " ";
        res += data[i];
    }
    return res;
}
