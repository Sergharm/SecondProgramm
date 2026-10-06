#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node* next;
};

int main()
{
    cout << "АДРЕСА ЭЛЕМЕНТОВ МАССИВА\n";
    cout << "==========================\n";

    int array[5] = {10, 20, 30, 40, 50};

    for (int i = 0; i < 5; i++)
    {
        cout << "array[" << i << "] = "
             << array[i]
             << " | address = "
             << &array[i]
             << endl;
    }


    cout << "\nАДРЕСА УЗЛОВ СПИСКА\n";
    cout << "====================\n";

    Node* first = new Node{10, nullptr};
    Node* second = new Node{20, nullptr};
    Node* third = new Node{30, nullptr};

    first->next = second;
    second->next = third;

    Node* current = first;

    while (current != nullptr)
    {
        cout << "data = "
             << current->data
             << " | address = "
             << current
             << " | next = "
             << current->next
             << endl;

        current = current->next;
    }


    cout << "\nСравнение адресов:\n";

    cout << "Адрес first:  " << first << endl;
    cout << "Адрес second: " << second << endl;
    cout << "Адрес third:  " << third << endl;


    delete first;
    delete second;
    delete third;

    return 0;
}