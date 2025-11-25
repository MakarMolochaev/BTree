#include "BTree.h"


void BTree::Insert(int value) {
    if (!this->root) {
        root = std::make_unique<Node>();
        root->AddKey(value);
        return;
    }

    if (root->keys.size() == 2 * t - 1) {
        auto [midKey, right] = root->Split(this->t);

        auto new_root = std::make_unique<Node>();
        new_root->keys.push_back(midKey);
        new_root->children.push_back(std::move(this->root));
        new_root->children.push_back(std::move(right));

        this->root = std::move(new_root);
    }

    insertNonFull(this->root.get(), value);
}

bool BTree::Search(int value) {
    return this->searchInNode(this->root, value);
}

bool BTree::searchInNode(std::unique_ptr<Node>& node, int value) {
    if (!node) {
        return false;
    }
    
    if(node->HasKey(value)) {
        return true;
    }
    int childIndex = node->GetIndexFor(value);
    return this->searchInNode(node->children[childIndex], value);
}

void BTree::insertNonFull(Node* node, int key) {
    int idx = node->GetIndexFor(key);
    if (idx < node->keys.size() && node->keys[idx] == key) {
        return;
    }
    if (node->children.empty()) {
        node->AddKey(key);
    } else {
        Node* child = node->children[idx].get();
        if (child->keys.size() == 2 * this->t - 1) {
            auto [mid, right] = child->Split(this->t);

            node->keys.insert(node->keys.begin() + idx, mid);

            node->children.insert(node->children.begin() + idx + 1, std::move(right));

            if (key > mid) {
                idx++;
            }
            child = node->children[idx].get();
        }

        insertNonFull(child, key);
    }
}


void BTree::PrettyPrint() {
    if (!root) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }
    prettyPrintNode(root.get(), 0, "");
}

void BTree::prettyPrintNode(Node* node, int level, const std::string& prefix) {
    if (!node) return;
    
    std::string indent;
    for (int i = 0; i < level; ++i) {
        indent += "    ";
    }
    
    std::cout << indent << prefix << "[";
    for (size_t i = 0; i < node->keys.size(); ++i) {
        std::cout << node->keys[i];
        if (i < node->keys.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "]" << std::endl;
        
    for (size_t i = 0; i < node->children.size(); ++i) {
        std::string childPrefix;
        if (i == node->children.size() - 1) {
            childPrefix = "\\-- ";
        } else {
            childPrefix = "|-- ";
        }
        prettyPrintNode(node->children[i].get(), level + 1, childPrefix);
    }
}

void BTree::remove(int key) {
    if (!root) return;
    Remove(this->root.get(), key);
}

void BTree::Remove(Node* node, int key) {
    if (!node) return;

    int idx = node->GetIndexFor(key);

    if (idx < static_cast<int>(node->keys.size()) && node->keys[idx] == key) {
        if (node->IsLeaf()) {
            node->RemoveKey(idx);
        } else {
            this->removeFromNode(node, idx);
        }
    } else {
        if (node->IsLeaf()) return;

        Node* child = node->children[idx].get();
        bool flag = (child->keys.size() == t - 1);

        this->Remove(child, key);

        if (flag && child->keys.size() < t - 1) {
            if (idx != 0 && node->children[idx - 1]->keys.size() >= t) {
                borrowFromPrev(node, idx);
            } else if (idx != node->keys.size() && node->children[idx + 1]->keys.size() >= t) {
                borrowFromNext(node, idx);
            } else {
                if (idx != node->keys.size()) {
                    merge(node, idx);
                } else {
                    merge(node, idx - 1);
                }
            }
        }
    }

    if (node == root.get() && node->keys.empty() && !node->children.empty()) {
        root = std::move(node->children[0]);
    }
}

void BTree::removeFromNode(Node* node, int idx) {
    int key = node->keys[idx];

    Node* predChild = node->children[idx].get();
    if (predChild->keys.size() >= static_cast<size_t>(t)) {
        int pred = predChild->GetMax();
        node->keys[idx] = pred;
        Remove(predChild, pred);
    } else {
        Node* succChild = node->children[idx + 1].get();
        if (succChild->keys.size() >= static_cast<size_t>(t)) {
            int succ = succChild->GetMin();
            node->keys[idx] = succ;
            Remove(succChild, succ);
        } else {
            merge(node, idx);
            Remove(node->children[idx].get(), key);
        }
    }
}

void BTree::merge(Node* node, int idx) {
    Node* child = node->children[idx].get();
    Node* sibling = node->children[idx + 1].get();

    child->keys.push_back(node->keys[idx]);

    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

    if (!child->IsLeaf()) {
        child->children.insert(child->children.end(),
            std::make_move_iterator(sibling->children.begin()),
            std::make_move_iterator(sibling->children.end()));
    }

    node->RemoveKey(idx);
    node->RemoveChild(idx + 1);
}

void BTree::borrowFromPrev(Node* node, int idx) {
    Node* child = node->children[idx].get();
    Node* sibling = node->children[idx - 1].get();

    child->keys.insert(child->keys.begin(), node->keys[idx - 1]);
    if (!child->IsLeaf()) {
        child->children.insert(child->children.begin(), std::move(sibling->children.back()));
    }

    node->keys[idx - 1] = sibling->keys.back();

    sibling->RemoveKey(sibling->keys.size() - 1);
    if (!sibling->IsLeaf()) {
        sibling->RemoveChild(sibling->children.size() - 1);
    }
}

void BTree::borrowFromNext(Node* node, int idx) {
    Node* child = node->children[idx].get();
    Node* sibling = node->children[idx + 1].get();

    child->keys.push_back(node->keys[idx]);
    if (!child->IsLeaf()) {
        child->children.push_back(std::move(sibling->children[0]));
    }

    node->keys[idx] = sibling->keys[0];

    sibling->RemoveKey(0);
    if (!sibling->IsLeaf()) {
        sibling->RemoveChild(0);
    }
}