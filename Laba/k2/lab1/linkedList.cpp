#include <iostream>
#include <string>
#include "linkedList.h"
using namespace std;

linkedList::~linkedList(){
    clear();
}

void linkedList::pushFront(const string& value){
    Node* newHead = new Node;
    newHead->data = value;
    newHead->next = head;
    head = newHead;
    if (tail == nullptr){
        tail = head;
    }
}

void linkedList::pushBack(const string& value){ 
    Node* newTail = new Node;
    if (tail == nullptr){
        newTail->data = value;
        newTail->next = nullptr;
        tail = newTail;
        head = tail;
    } else {
        newTail->data = value;
        newTail->next = nullptr;
        tail->next = newTail;
        tail = newTail;
    }
}

void linkedList::Print(){
    Node* temp = head;
    while(temp != nullptr){
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

void linkedList::clear(){
    while(head != nullptr){
        Node* newHead = head->next;
        delete head;
        head =newHead;
    }
    tail = nullptr;
}

void linkedList::pushBefore(const string& tar, const string& value){
    if (head == nullptr) {
        return;
    }
    if (head->data == tar) {
        pushFront(value);
        return;
    }
    Node* temp = head;
    while (temp->next != nullptr && temp->next->data != tar) {
        temp = temp->next;
    }
    if (temp->next == nullptr) {
        return;
    }
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = temp->next;
    temp->next = newNode;
}

void linkedList::pushAfter(const string& tar, const string& value) {
    Node* temp = head;
    while (temp != nullptr && temp->data != tar) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        return;
    }
    if (temp == tail) { 
        pushBack(value);
        return;
    }
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = temp->next;
    temp->next = newNode;

}

void linkedList::GetHead() {
    if (head == nullptr) {
        return;
    }
    cout << head->data << endl;
}

void linkedList::GetTail() {
    if (tail == nullptr) {
        return;
    }
    cout << tail->data << endl;
}

void linkedList::GetIndx(const int& indx) {
    if (indx < 0) {
        return;
    }
    Node* temp = head;
    for (int i = 0; i < indx; i++) {
        if (temp == nullptr) {
            return;
        }
        temp = temp->next;
    }
    if (temp == nullptr) {
        return;
    }
    cout << temp->data << endl;
}

void linkedList::DelVal(const string& value) {
    if (head == nullptr) {
        return;
    }
    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        if (head == nullptr) {
            tail = nullptr;
        }
        return;
    }
    Node* temp = head;
    while (temp->next != nullptr && temp->next->data != value) {
        temp = temp->next;
    }
    if (temp->next == nullptr) {
        return;
    }
    Node* delNode = temp->next;
    temp->next = delNode->next;
    if (delNode == tail) {
        tail = temp;
    }
    delete delNode;
}

void linkedList::GetVal(const string& value) {
    Node* temp = head;
    int indx = 0;
    while (temp != nullptr) {
        if (temp->data == value) {
            cout << indx << endl;
            return;
        }
        temp = temp->next;
        indx++;
    }
}

void linkedList::delFront() {
    if (head == nullptr) {
        return;
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    if (head == nullptr) {
        tail = nullptr;
    }
}

void linkedList::delBack() {
    if (head == nullptr) {
        return;
    }
    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }
    Node* temp = head;
    while (temp->next != tail) {
        temp = temp->next;
    }
    delete tail;
    tail = temp;
    tail->next = nullptr;
}

void linkedList::delBefore(const string& tar) {
    if (head == nullptr || head->data == tar || head->next == nullptr) {
        return;
    }
    if (head->next->data == tar) {
        delFront();
        return;
    }
    Node* temp = head;
    while (temp->next->next != nullptr && temp->next->next->data != tar) {
        temp = temp->next;
    }
    if (temp->next->next == nullptr) {
        return;
    }
    Node* delNode = temp->next;
    temp->next = delNode->next;
    delete delNode;

}

void linkedList::delAfter(const string& tar) {
    Node* temp = head;
    while (temp != nullptr && temp->data != tar) {
        temp = temp->next;
    }
    if (temp == nullptr || temp->next == nullptr) {
        return;
    }
    Node* delNode = temp->next;
    temp->next = delNode->next;
    if (delNode == tail) {
        tail = temp;
    }
    delete delNode;

}

int linkedList::Size() {
    int count = 0;
    Node* temp = head;
    while (temp != nullptr) {
        ++count;
        temp = temp->next;
    }
    return count;
}
 
string linkedList::ToString() {
    string res;
    Node* temp = head;
    while (temp != nullptr) {
        if (!res.empty()) res += " ";
        res += temp->data;
        temp = temp->next;
    }
    return res;
}

string linkedList::At(const int& indx) {
    if (indx < 0) {
        return "";
    }

    Node* temp = head;
    for (int i = 0; i < indx; ++i) {
        if (temp == nullptr) {
            return "";
        }
        temp = temp->next;
    }

    if (temp == nullptr) {
        return "";
    }

    return temp->data;
}

int linkedList::Find(const string& value) {
    Node* temp = head;
    int indx = 0;

    while (temp != nullptr) {
        if (temp->data == value) {
            return indx;
        }

        temp = temp->next;
        ++indx;
    }

    return -1;
}