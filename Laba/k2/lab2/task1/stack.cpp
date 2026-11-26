#include <iostream>
#include "stack.h"

Stack::~Stack(){
    clear();
}

void Stack::push(const std::string& value){
    Node* newTop = new Node;
    newTop->value = value;
    newTop->next = top;
    top = newTop;
}

void Stack::pop(){
    if (top == nullptr){
        std::cout << "Error!" << std::endl;
        return;
    }
    Node* newTop = top->next;
    delete top;
    top = newTop;
}   

void Stack::peek(){
    if (top == nullptr){
        std::cout << "Пуст" << std::endl;
        return;
    }
    std::cout << top->value << std::endl;
}

void Stack::Print() {
    if (top == nullptr) {
        std::cout << "Пустой" << std::endl;
        return;
    }
    Node* temp = top;
    while (temp != nullptr) {
        std::cout << temp->value << std::endl;
        temp = temp->next;
    }
}

void Stack::clear(){
        while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

int Stack::Size(){
    int count = 0;
    Node* temp = top;
    while (temp != nullptr){
        ++count;
        temp = temp->next;
    }
    return count;
}

std::string Stack::ToString(){
    std::string res;
    Node* temp = top;
    while (temp != nullptr){
        if (temp == top){
            res = temp->value;
        } else {
            res = temp->value + " " + res;
        }
        temp = temp->next;
    }
    return res;
}
std::string Stack::Top(){
    return top == nullptr ? std::string() : top->value;
}

bool Stack::IsEmpty(){
    return top == nullptr;
}
