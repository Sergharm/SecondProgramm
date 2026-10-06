#include <iostream>
#include <queue>

using namespace std;

struct Node
{
    int data;

    Node* left;
    Node* right;
};

// Создание узла
Node* createNode(int value)
{
    return new Node{value, nullptr, nullptr};
}

// Вставка в бинарное дерево поиска
Node* insert(Node* root, int value)
{
    if (root == nullptr)
    {
        return createNode(value);
    }

    if (value < root->data)
    {
        root->left = insert(root->left, value);
    }
    else
    {
        root->right = insert(root->right, value);
    }

    return root;
}

// 1. Прямой обход
// Root -> Left -> Right
void preorder(Node* root)
{
    if (root == nullptr)
        return;

    cout << root->data << " ";

    preorder(root->left);
    preorder(root->right);
}

// 2. Симметричный обход
// Left -> Root -> Right
void inorder(Node* root)
{
    if (root == nullptr)
        return;

    inorder(root->left);

    cout << root->data << " ";

    inorder(root->right);
}

// 3. Обратный обход
// Left -> Right -> Root
void postorder(Node* root)
{
    if (root == nullptr)
        return;

    postorder(root->left);
    postorder(root->right);

    cout << root->data << " ";
}

// 4. BFS
void BFS(Node* root)
{
    if (root == nullptr)
        return;

    queue<Node*> q;

    q.push(root);

    while (!q.empty())
    {
        Node* current = q.front();

        q.pop();

        cout << current->data << " ";

        if (current->left != nullptr)
        {
            q.push(current->left);
        }

        if (current->right != nullptr)
        {
            q.push(current->right);
        }
    }
}

// Вывод адресов
void printAddresses(Node* root)
{
    if (root == nullptr)
        return;

    cout << "Node = " << root->data
         << " | address = " << root
         << " | left = " << root->left
         << " | right = " << root->right
         << endl;

    printAddresses(root->left);
    printAddresses(root->right);
}

// Очистка дерева
void clearTree(Node* root)
{
    if (root == nullptr)
        return;

    clearTree(root->left);
    clearTree(root->right);

    delete root;
}

int main()
{
    Node* root = nullptr;

    // Создаём дерево
    root = insert(root, 15);
    root = insert(root, 10);
    root = insert(root, 25);
    root = insert(root, 8);
    root = insert(root, 12);
    root = insert(root, 20);
    root = insert(root, 30);

    /*
             15
            /  \
          10    25
         /  \   / \
        8   12 20 30
    */

    cout << "Бинарное дерево поиска:\n\n";

    cout << "        15\n";
    cout << "       /  \\\n";
    cout << "     10    25\n";
    cout << "    /  \\   / \\\n";
    cout << "   8   12 20 30\n";


    cout << "\n\nPreorder:\n";
    preorder(root);

    cout << "\n\nInorder:\n";
    inorder(root);

    cout << "\n\nPostorder:\n";
    postorder(root);

    cout << "\n\nBFS:\n";
    BFS(root);

    cout << "\n\nАдреса узлов:\n";
    printAddresses(root);

    clearTree(root);

    return 0;
}