#pragma once
#include <iostream>
#include <string>

using namespace std;

// Complete Binary Tree (полное по уровням бинарное дерево):
// все уровни, кроме последнего, заполнены целиком,
// а последний уровень заполняется слева направо без пропусков
struct CBTree {
private:
    struct Node {
        int key = 0;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    Node* root = nullptr;

    void clear(Node* node);
    void printTree(Node* node, int depth);
    void inorder(Node* node);
    void preorder(Node* node);
    void postorder(Node* node);
    int countNodes(Node* node);
    int maxIndex(Node* node, int indx); // наибольший номер позиции при нумерации по уровням
    void fillLevels(Node* node, int indx, Node** cells); // раскладываем узлы по номерам позиций
public:
    ~CBTree();
    void Insert(int key); // добавление элемента в первое свободное место по уровням
    bool Contains(int key); // поиск элемента
    bool IsComplete(); // проверка, является ли дерево Complete
    void Print(); // вывод дерева боком
    void PrintInorder(); // обход в глубину: левое, корень, правое
    void PrintPreorder(); // обход в глубину: корень, левое, правое
    void PrintPostorder(); // обход в глубину: левое, правое, корень
    void PrintBFS(); // обход в ширину (по уровням)
    int Size(); // количество элементов
    bool Load(const string& data); // восстановление дерева при чтении файла
    string ToString(); // ключи по уровням для записи в файл ("-" — пустое место)
};
