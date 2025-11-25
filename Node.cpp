#include "Node.h"
#include <vector>
#include <algorithm>
#include <iostream>

void Node::AddKey(int value) {
    auto it = std::lower_bound(this->keys.begin(), this->keys.end(), value);
    this->keys.insert(it, value);
}

int Node::GetIndexFor(int value) {
    auto it = std::lower_bound(this->keys.begin(), this->keys.end(), value);
    return std::distance(this->keys.begin(), it);
}

bool Node::HasKey(int value) {
    return std::binary_search(this->keys.begin(), this->keys.end(), value);
}

std::tuple<int, std::unique_ptr<Node>> Node::Split(int t) {
    auto right = std::make_unique<Node>();

    int mid_key = keys[t - 1];

    right->keys.resize(keys.size() - t);
    std::move(keys.begin() + t, keys.end(), right->keys.begin());
    keys.resize(t - 1);

    if (!children.empty()) {
        right->children.resize(t);
        std::move(children.begin() + t, children.end(), right->children.begin());

        children.resize(t);
    }

    return {mid_key, std::move(right)};
}