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
    private:
        bool searchInNode(std::unique_ptr<Node>& node, int value);
        void insertNonFull(Node* node, int key);
        void prettyPrintNode(Node* node, int level, const std::string& prefix);
};

#endif