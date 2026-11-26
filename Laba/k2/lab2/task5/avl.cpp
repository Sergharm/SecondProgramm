#include "avl.h"
#include <algorithm>
#include <cstdlib>

// Вставка ключа в бинарное дерево поиска (повторы пропускаются)
void insertKey(std::unique_ptr<AvlNode>& node, int key) {
    if (!node) {
        node = std::make_unique<AvlNode>(key);
        return;
    }
    if (key < node->key) {
        insertKey(node->left, key);
    } else if (key > node->key) {
        insertKey(node->right, key);
    }
}

// Вычисление высоты поддерева и проверка условия АВЛ-сбалансированности
int heightAndCheck(const AvlNode* node, bool& ok) {
    if (node == nullptr || !ok) {
        return 0;
    }

    int hl = heightAndCheck(node->left.get(), ok);
    if (!ok) return 0;

    int hr = heightAndCheck(node->right.get(), ok);
    if (!ok) return 0;

    if (std::abs(hl - hr) > 1) {
        ok = false;
    }

    return std::max(hl, hr) + 1;
}
