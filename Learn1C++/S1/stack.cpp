#include <iostream>
#include <string>

using namespace std;

struct Node
{
    string person;
    Node* next;
};

struct Stack
{
    Node* head;
};

// Инициализация
void InitStack(Stack* stack)
{
    stack->head = nullptr;
}

// Проверка на пустоту
bool isEmptyStack(Stack* stack)
{
    return stack->head == nullptr;
}

// Добавление
void push(Stack* stack, const string& str)
{
    Node* newNode = new Node{str, stack->head};

    stack->head = newNode;
}

// Удаление
void pop(Stack* stack)
{
    if (isEmptyStack(stack))
    {
        cout << "Стек пуст\n";
        return;
    }

    Node* deleteNode = stack->head;

    stack->head = stack->head->next;

    delete deleteNode;
}

// Получение верхнего элемента
string top(Stack* stack)
{
    if (isEmptyStack(stack))
        return "";

    return stack->head->person;
}

// Вывод
void printStack(Stack* stack)
{
    Node* current = stack->head;

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
void clearStack(Stack* stack)
{
    while (!isEmptyStack(stack))
    {
        pop(stack);
    }
}

int main()
{
    Stack stack;

    InitStack(&stack);

    push(&stack, "Sovunya");
    push(&stack, "Krosh");
    push(&stack, "Ejik");

    cout << "Стек:\n";
    printStack(&stack);

    cout << "\nВерхний элемент: "
         << top(&stack)
         << endl;

    cout << "\nУдаляем элемент:\n";

    pop(&stack);

    printStack(&stack);

    clearStack(&stack);

    return 0;
}