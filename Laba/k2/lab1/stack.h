#pragma once
#include <iostream>
#include <string>

struct Stack {
private:
    struct Node {
        std::string value;
        Node* next = nullptr;
    };
    Node* top = nullptr;
    void clear();
public:
    ~Stack();
    void push(const std::string& value);
    void pop();
    void Print();
    void peek(); 
    int Size();
    std::string ToString();
};