#include <iostream>
#include <sstream>
#include "CBTree.h"

CBTree::~CBTree() {
    clear(root);
}

void CBTree::clear(Node* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

// Добавление: идём по уровням слева направо и ставим новый узел
// на первое свободное место — так дерево остаётся Complete
void CBTree::Insert(int key) {
    Node* node = new Node;
    node->key = key;
    if (root == nullptr) {
        root = node;
        return;
    }
    int n = Size();
    Node** q = new Node*[n]; // очередь для обхода в ширину
    int head = 0;
    int tail = 0;
    q[tail] = root;
    ++tail;
    while (head < tail) {
        Node* temp = q[head];
        ++head;
        if (temp->left == nullptr) {
            temp->left = node; // свободно место слева
            break;
        }
        if (temp->right == nullptr) {
            temp->right = node; // свободно место справа
            break;
        }
        q[tail] = temp->left;
        ++tail;
        q[tail] = temp->right;
        ++tail;
    }
    delete[] q;
}

// Поиск: дерево не упорядочено, поэтому просматриваем все узлы обходом в ширину
bool CBTree::Contains(int key) {
    if (root == nullptr) {
        return false;
    }
    int n = Size();
    Node** q = new Node*[n];
    int head = 0;
    int tail = 0;
    q[tail] = root;
    ++tail;
    bool found = false;
    while (head < tail) {
        Node* node = q[head];
        ++head;
        if (node->key == key) {
            found = true;
            break;
        }
        if (node->left != nullptr) {
            q[tail] = node->left;
            ++tail;
        }
        if (node->right != nullptr) {
            q[tail] = node->right;
            ++tail;
        }
    }
    delete[] q;
    return found;
}

// Проверка на Complete: при обходе в ширину после первого пустого места
// не должно встретиться ни одного узла
bool CBTree::IsComplete() {
    if (root == nullptr) {
        return true; // пустое дерево считается Complete
    }
    int n = Size();
    Node** q = new Node*[n];
    int head = 0;
    int tail = 0;
    q[tail] = root;
    ++tail;
    bool gap = false; // уже встретили пустое место
    bool complete = true;
    while (head < tail && complete) {
        Node* node = q[head];
        ++head;
        if (node->left != nullptr) {
            if (gap) {
                complete = false; // узел стоит после пропуска
            }
            q[tail] = node->left;
            ++tail;
        } else {
            gap = true;
        }
        if (node->right != nullptr) {
            if (gap) {
                complete = false;
            }
            q[tail] = node->right;
            ++tail;
        } else {
            gap = true;
        }
    }
    delete[] q;
    return complete;
}

void CBTree::printTree(Node* node, int depth) {
    if (node == nullptr) {
        return;
    }
    printTree(node->right, depth + 1);
    for (int i = 0; i < depth; ++i) {
        cout << "      ";
    }
    cout << node->key << endl;
    printTree(node->left, depth + 1);
}

void CBTree::Print() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    printTree(root, 0);
}

void CBTree::inorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    inorder(node->left);
    cout << node->key << " ";
    inorder(node->right);
}

void CBTree::preorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    cout << node->key << " ";
    preorder(node->left);
    preorder(node->right);
}

void CBTree::postorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    postorder(node->left);
    postorder(node->right);
    cout << node->key << " ";
}

void CBTree::PrintInorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    inorder(root);
    cout << endl;
}

void CBTree::PrintPreorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    preorder(root);
    cout << endl;
}

void CBTree::PrintPostorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    postorder(root);
    cout << endl;
}

void CBTree::PrintBFS() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    int n = Size();
    Node** q = new Node*[n];
    int head = 0;
    int tail = 0;
    q[tail] = root;
    ++tail;
    while (head < tail) {
        Node* node = q[head];
        ++head;
        cout << node->key << " ";
        if (node->left != nullptr) {
            q[tail] = node->left;
            ++tail;
        }
        if (node->right != nullptr) {
            q[tail] = node->right;
            ++tail;
        }
    }
    cout << endl;
    delete[] q;
}

int CBTree::countNodes(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int CBTree::Size() {
    return countNodes(root);
}

// В файле ключи записаны по уровням: элемент с номером i — ребёнок элемента (i - 1) / 2.
// Нечётный номер — левый ребёнок, чётный — правый, "-" означает пустое место.
// Возвращает false, если данные повреждены
bool CBTree::Load(const string& data) {
    int count = 0; // сколько слов в строке
    stringstream cs(data);
    string w;
    while (cs >> w) {
        ++count;
    }
    if (count == 0) {
        return true;
    }
    Node** cells = new Node*[count];
    for (int i = 0; i < count; ++i) {
        cells[i] = nullptr;
    }
    bool ok = true;
    stringstream ss(data);
    for (int i = 0; i < count && ok; ++i) {
        ss >> w;
        if (w == "-") {
            continue; // пустое место
        }
        int len = w.length();
        int start = 0;
        if (len > 0 && w[0] == '-') {
            start = 1;
        }
        if (len - start < 1 || len - start > 9) {
            ok = false;
        }
        for (int j = start; j < len && ok; ++j) {
            if (w[j] < '0' || w[j] > '9') {
                ok = false; // ключ должен быть целым числом
            }
        }
        if (!ok) {
            break;
        }
        if (i > 0 && cells[(i - 1) / 2] == nullptr) {
            ok = false; // у узла нет родителя
            break;
        }
        Node* node = new Node;
        node->key = stoi(w);
        cells[i] = node;
        if (i == 0) {
            root = node;
        } else if (i % 2 == 1) {
            cells[(i - 1) / 2]->left = node;
        } else {
            cells[(i - 1) / 2]->right = node;
        }
    }
    if (ok && cells[0] == nullptr) {
        ok = false; // нет корня
    }
    delete[] cells;
    return ok;
}

int CBTree::maxIndex(Node* node, int indx) {
    if (node == nullptr) {
        return -1;
    }
    int res = indx;
    int l = maxIndex(node->left, 2 * indx + 1);
    int r = maxIndex(node->right, 2 * indx + 2);
    if (l > res) {
        res = l;
    }
    if (r > res) {
        res = r;
    }
    return res;
}

void CBTree::fillLevels(Node* node, int indx, Node** cells) {
    if (node == nullptr) {
        return;
    }
    cells[indx] = node;
    fillLevels(node->left, 2 * indx + 1, cells);
    fillLevels(node->right, 2 * indx + 2, cells);
}

string CBTree::ToString() {
    string res;
    if (root == nullptr) {
        return res;
    }
    int n = maxIndex(root, 0) + 1; // у Complete дерева совпадает с количеством узлов
    Node** cells = new Node*[n];
    for (int i = 0; i < n; ++i) {
        cells[i] = nullptr;
    }
    fillLevels(root, 0, cells);
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            res += " ";
        }
        if (cells[i] == nullptr) {
            res += "-";
        } else {
            res += to_string(cells[i]->key);
        }
    }
    delete[] cells;
    return res;
}
