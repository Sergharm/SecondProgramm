#include <iostream>
#include "Queue.h"

Queue::~Queue(){
    clear();
}

void Queue::push(const std::string& value){
    Node* newTail = new Node;
    newTail->value = value;
    if (tail == nullptr){
        head = tail = newTail;
        return;
    }
    tail->next = newTail;
    tail = newTail;
}

void Queue::pop(){
    if (head == nullptr){ // Возвращение элемента
        std::cout << "Error!" << std::endl;
        return;
    }
    Node* newHead = head->next;
    delete head;
    head = newHead;
    if (head == nullptr){
        tail = nullptr; // удалили последний элемент
    }
}

void Queue::peek(){
    if (head == nullptr){
        std::cout << "Пуста" << std::endl;
        return;
    }
    std::cout << head->value << std::endl;
}

void Queue::Print(){
    if (head == nullptr){
        std::cout << "Пустая" << std::endl;
        return;
    }
    Node* temp = head;
    while (temp != nullptr){
        std::cout << temp->value << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

void Queue::clear(){
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

int Queue::Size(){
    int count = 0;
    Node* temp = head;
    while (temp != nullptr){
        ++count;
        temp = temp->next;
    }
    return count;
}

std::string Queue::ToString(){
    std::string res;
    Node* temp = head;
    while (temp != nullptr){
        if (temp != head){
            res += " ";
        }
        res += temp->value;
        temp = temp->next;
    }
    return res;
}