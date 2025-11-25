#ifndef BTREE_H
#define BTREE_H
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Node.h"

class BTree {
    private:
        std::unique_ptr<Node> root;
        size_t t;
        size_t m;
    public:
        BTree(size_t t = 2) : t(t), m(2*t) {}
        ~BTree() = default;
        void Insert(int value);
        bool Search(int value);
        void PrettyPrint();
        void remove(int value);
    private:
        void Remove(Node* node, int key);
        void merge(Node* node, int idx);
        void borrowFromPrev(Node* node, int idx);
        void borrowFromNext(Node* node, int idx);
        void removeFromNode(Node* node, int idx);
        bool searchInNode(std::unique_ptr<Node>& node, int value);
        void insertNonFull(Node* node, int key);
        void prettyPrintNode(Node* node, int level, const std::string& prefix);
};

#endif