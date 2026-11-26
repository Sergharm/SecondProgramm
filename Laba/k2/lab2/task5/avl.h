#pragma once
#include <memory>

// Узел бинарного дерева поиска
struct AvlNode {
    int key;
    std::unique_ptr<AvlNode> left;
    std::unique_ptr<AvlNode> right;
    explicit AvlNode(int k) : key(k) {}
};

// Вставка в дерево поиска (повторы пропускаются)
void insertKey(std::unique_ptr<AvlNode>& node, int key);

// Высота поддерева; ok сбрасывается в false, если в каком-либо узле высоты поддеревьев различаются больше чем на 1
int heightAndCheck(const AvlNode* node, bool& ok);
