#include <iostream>
#include <string>

using namespace std;

struct Node
{
    string person;
    Node* next;
};

struct Queue
{
    Node* head;
    Node* tail;
};

// Инициализация
void InitQueue(Queue* q)
{
    q->head = nullptr;
    q->tail = nullptr;
}

// Проверка
bool isEmptyQueue(Queue* q)
{
    return q->head == nullptr;
}

// Добавление в конец
void insert(Queue* q, const string& str)
{
    Node* newNode = new Node{str, nullptr};

    if (q->head == nullptr)
    {
        q->head = newNode;
        q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

// Удаление из начала
void remove(Queue* q)
{
    if (isEmptyQueue(q))
    {
        cout << "Очередь пуста\n";
        return;
    }

    Node* deleteNode = q->head;

    q->head = q->head->next;

    if (q->head == nullptr)
    {
        q->tail = nullptr;
    }

    delete deleteNode;
}

// Вывод
void printQueue(Queue* q)
{
    Node* current = q->head;

    while (current != nullptr)
    {
        cout << current->person
             << " | address: " << current
             << " | next: " << current->next
             << endl;

        current = current->next;
    }
}

// Очистка
void clearQueue(Queue* q)
{
    while (!isEmptyQueue(q))
    {
        remove(q);
    }
}

int main()
{
    Queue queue;

    InitQueue(&queue);

    insert(&queue, "Nusha");
    insert(&queue, "Krosh");
    insert(&queue, "Barash");

    cout << "Очередь:\n";
    printQueue(&queue);

    cout << "\nУдаляем первый элемент:\n";

    remove(&queue);

    printQueue(&queue);

    clearQueue(&queue);

    return 0;
}