#include <iostream>
#include <string>

using namespace std;

struct Node
{
    string person;

    Node* prev;
    Node* next;
};

struct Deque
{
    Node* head;
    Node* tail;
};

// Инициализация
void InitDeque(Deque* dq)
{
    dq->head = nullptr;
    dq->tail = nullptr;
}

// Проверка
bool isEmpty(Deque* dq)
{
    return dq->head == nullptr;
}

// Добавление в начало
void pushFront(Deque* dq, const string& str)
{
    Node* newNode =
        new Node{str, nullptr, dq->head};

    if (dq->head == nullptr)
    {
        dq->head = newNode;
        dq->tail = newNode;
    }
    else
    {
        dq->head->prev = newNode;
        dq->head = newNode;
    }
}

// Добавление в конец
void pushBack(Deque* dq, const string& str)
{
    Node* newNode =
        new Node{str, dq->tail, nullptr};

    if (dq->tail == nullptr)
    {
        dq->head = newNode;
        dq->tail = newNode;
    }
    else
    {
        dq->tail->next = newNode;
        dq->tail = newNode;
    }
}

// Удаление с начала
void popFront(Deque* dq)
{
    if (isEmpty(dq))
        return;

    Node* deleteNode = dq->head;

    dq->head = dq->head->next;

    if (dq->head != nullptr)
    {
        dq->head->prev = nullptr;
    }
    else
    {
        dq->tail = nullptr;
    }

    delete deleteNode;
}

// Удаление с конца
void popBack(Deque* dq)
{
    if (isEmpty(dq))
        return;

    Node* deleteNode = dq->tail;

    dq->tail = dq->tail->prev;

    if (dq->tail != nullptr)
    {
        dq->tail->next = nullptr;
    }
    else
    {
        dq->head = nullptr;
    }

    delete deleteNode;
}

// Вывод
void printDeque(Deque* dq)
{
    Node* current = dq->head;

    while (current != nullptr)
    {
        cout << current->person << " ";

        current = current->next;
    }

    cout << endl;
}

// Очистка
void clearDeque(Deque* dq)
{
    while (!isEmpty(dq))
    {
        popFront(dq);
    }
}

int main()
{
    Deque deque;

    InitDeque(&deque);

    pushBack(&deque, "A");
    pushBack(&deque, "B");
    pushFront(&deque, "START");
    pushBack(&deque, "C");

    cout << "Дек:\n";

    printDeque(&deque);

    cout << "\nУдаляем с начала:\n";

    popFront(&deque);

    printDeque(&deque);

    cout << "\nУдаляем с конца:\n";

    popBack(&deque);

    printDeque(&deque);

    clearDeque(&deque);

    return 0;
}