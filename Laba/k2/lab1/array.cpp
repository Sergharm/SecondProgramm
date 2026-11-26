#include <iostream>
#include <string>
#include "array.h"
using namespace std;

Array::~Array(){
    clear();
}

Array::Array(){
    Create();
}

void Array::Create(){
    clear();
    highlighted = 10;
    data = new string[highlighted];
    isUsed = 0;
}

void Array::increaseArrasy(){
    highlighted *= 2;
    string* newData = new string [highlighted];
    for(int i = 0; i<isUsed; ++i){
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

void Array::PushBack(const string& value){
    if (isUsed == highlighted){
        increaseArrasy();
    }
    data[isUsed] = value;
    ++isUsed;
}

void Array::Print(){
    for(int i = 0; i<isUsed; ++i){
        cout << data[i]<<" ";
    }
    cout << endl;
}

void Array::Insert(const int& indx, const string& value){
    if (isUsed == highlighted){
        increaseArrasy();
    }
    if (isUsed < indx || indx < 0){
        return;
    }
    for (int i = isUsed; indx < i; --i){
        data[i] = data[i-1];
    }
    data[indx] = value;
    ++isUsed;
}

void Array::clear(){
    delete[] data;
    data = nullptr;
    isUsed = 0;
}

void Array::Length(){
    cout << "Длинна массива: " << isUsed << endl;
}

void Array::Get(const int& indx){
    if (indx < 0 || indx >=isUsed){
        return;
    }
    cout << data[indx]<<endl;
}

void Array::DelIndx(const int& indx){
     if (indx < 0 || indx >= isUsed) {
        return;
    }
    for (int i = indx; i < isUsed - 1; ++i) {
        data[i] = data[i + 1];
    }
    --isUsed;
}

void Array::Set(int indx, string val){
    if (indx < 0 || indx >= isUsed){
        return;
    }
    data[indx] = val;
}

int Array::Size(){
    return isUsed;
}
 
string Array::At(const int& indx){
    if (indx < 0 || indx >= isUsed){
        return "";
    }
    return data[indx];
}
 
string Array::ToString(){
    string res;
    for (int i = 0; i < isUsed; ++i){
        if (i > 0) res += " ";
        res += data[i];
    }
    return res;
}
