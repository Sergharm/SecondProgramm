#include <iostream>
#include <string>

using namespace std;

struct Node
{
    string person;
    Node* next;
};

// Вывод списка
void printList(Node* head)
{
    Node* current = head;

    while (current != nullptr)
    {
        cout << current->person
             << " | address: " << current
             << " | next: " << current->next
             << endl;

        current = current->next;
    }
}

// Добавление в начало
void addFirst(Node*& head, const string& str)
{
    Node* newNode = new Node{str, head};
    head = newNode;
}

// Добавление в конец
void addLast(Node*& head, const string& str)
{
    Node* newNode = new Node{str, nullptr};

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
}

// Добавление после указанного узла
void addAfter(Node* node, const string& str)
{
    if (node == nullptr)
        return;

    Node* newNode = new Node{str, node->next};

    node->next = newNode;
}

// Добавление по индексу
void addAt(Node*& head, int index, const string& str)
{
    if (index < 0)
        return;

    if (index == 0)
    {
        addFirst(head, str);
        return;
    }

    Node* current = head;

    for (int i = 0; i < index - 1 && current != nullptr; i++)
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "Неверный индекс\n";
        return;
    }

    addAfter(current, str);
}

// Удаление первого элемента
void deleteFirst(Node*& head)
{
    if (head == nullptr)
        return;

    Node* deleteNode = head;

    head = head->next;

    delete deleteNode;
}

// Удаление после указанного узла
void deleteAfter(Node* node)
{
    if (node == nullptr || node->next == nullptr)
        return;

    Node* deleteNode = node->next;

    node->next = deleteNode->next;

    delete deleteNode;
}

// Очистка списка
void clearList(Node*& head)
{
    while (head != nullptr)
    {
        deleteFirst(head);
    }
}

int main()
{
    Node* head = nullptr;

    addLast(head, "Nusha");
    addLast(head, "Kopatych");
    addLast(head, "Ejik");

    cout << "Исходный список:\n";
    printList(head);

    cout << "\nДобавление в начало:\n";

    addFirst(head, "Krosh");

    printList(head);

    cout << "\nДобавление после Head:\n";

    addAfter(head, "Sovunya");

    printList(head);

    cout << "\nДобавление по индексу 2:\n";

    addAt(head, 2, "Barash");

    printList(head);

    cout << "\nУдаление первого элемента:\n";

    deleteFirst(head);

    printList(head);

    clearList(head);

    return 0;
}