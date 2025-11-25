#ifndef NODE_H
#define NODE_H
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Node {
    public:
        std::vector<int> keys;
        std::vector<std::unique_ptr<Node>> children;
        Node() {}
        int GetIndexFor(int value);
        void AddKey(int value);
        bool HasKey(int value);
        std::tuple<int, std::unique_ptr<Node>> Split(int t);
};

#endif