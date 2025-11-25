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