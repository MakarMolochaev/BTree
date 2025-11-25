#include <iostream>
#include "BTree.h"

int main() {
    BTree tree(2);
    tree.Insert(5);
    tree.Insert(6);
    tree.Insert(7);
    tree.Insert(8);
    tree.Insert(9);
    tree.Insert(10);
    tree.Insert(11);
    tree.Insert(12);
    tree.Insert(13);
    tree.Insert(18);
    tree.Insert(15);
    tree.Insert(14);
    std::cout << tree.Search(5) << std::endl;
    tree.PrettyPrint();
    std::cout << std::endl;
    tree.remove(8);
    tree.PrettyPrint();
    return 0;
}