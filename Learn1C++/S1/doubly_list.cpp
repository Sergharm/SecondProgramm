#include <iostream>
#include <string>

using namespace std;

struct Node
{
    string person;

    Node* prev;
    Node* next;
};

// Добавление в начало
void addFirst(Node*& head, const string& str)
{
    Node* newNode = new Node{str, nullptr, head};

    if (head != nullptr)
    {
        head->prev = newNode;
    }

    head = newNode;
}

// Добавление в конец
void addLast(Node*& head, const string& str)
{
    Node* newNode = new Node{str, nullptr, nullptr};

    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    Node* current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
    newNode->prev = current;
}

// Вывод вперёд
void printForward(Node* head)
{
    Node* current = head;

    while (current != nullptr)
    {
        cout << current->person
             << " | address: " << current
             << " | prev: " << current->prev
             << " | next: " << current->next
             << endl;

        current = current->next;
    }
}

// Вывод назад
void printBackward(Node* head)
{
    if (head == nullptr)
        return;

    Node* current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    cout << "\nВ обратном порядке:\n";

    while (current != nullptr)
    {
        cout << current->person << endl;

        current = current->prev;
    }
}

// Очистка
void clearList(Node*& head)
{
    while (head != nullptr)
    {
        Node* deleteNode = head;

        head = head->next;

        delete deleteNode;
    }
}

int main()
{
    Node* head = nullptr;

    addLast(head, "Nusha");
    addLast(head, "Krosh");
    addLast(head, "Ejik");

    cout << "Двусвязный список:\n";

    printForward(head);

    printBackward(head);

    clearList(head);

    return 0;
}