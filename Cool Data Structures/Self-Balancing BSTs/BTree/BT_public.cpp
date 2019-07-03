#include "BTree.h"
#include <bits/stdc++.h>
#include <utility>

template <typename T>
BTree<T>::BTree(unsigned min_d, bool (*compare)(T, T), void (*printK)(T)) {
    min_degree = min_d;

    root = (BNode<T>*) malloc(sizeof(BNode<T>));
    initialize_Node(this->root);

    root->leaf = true;

    print_key= printK;
    less_than = compare;
}

template <typename T>
BTree<T>::~BTree<T>() {
    free_Node(this->root);
}

template <typename T>
void BTree<T>::insert(T k) {

    // Grow upwards if the root is full.
    if (root->size == 2 * min_degree - 1) {
        BNode<T> *newRoot = (BNode<T>*) malloc(sizeof(BNode<T>));
        initialize_Node(newRoot);
        newRoot->leaf = false;
        newRoot->child[0] = root;
        root = newRoot;

        split_Child(newRoot, 0);
    }

    BNode<T> *curr = root;
    while (!curr->leaf) {

        // Find the proper child to go to.
        int index = curr->size - 1;
        while (index >= 0 && less_than(k, curr->key[index])) {
            index--;
        }
        index++;

        // Split child if full.
        if (curr->child[index]->size == 2 * min_degree - 1) {
            split_Child(curr, index);

            if (less_than(curr->key[index], k)) {
                ++index;
            }
        }
        curr = curr->child[index];
    }

    insert_Node(curr, k);
}

// Removes k from the tree. Returns the removed key.
// Throws a BTREE_EXCEPTION if key is not found.
template <typename T>
T BTree<T>::remove(T k) {
    BNode<T> *curr = root;
    while (true) {
        unsigned i = findIndex(curr, k);

        // If the item to be deleted has been found.
        if (i < curr->size && !(less_than(curr->key[i], k) || less_than(k, curr->key[i]))) {
            T toReturn = curr->key[i];

            // If at a leaf, just delete it.
            if (curr->leaf) {
                delete_Node(curr, i);
            }

                // Otherwise replace with predecessor/successor or merge children.
            else {
                BNode<T> *leftKid = curr->child[i];
                BNode<T> *rightKid = curr->child[i + 1];

                // Replace with predecessor.
                if (leftKid->size >= min_degree) {
                    while (!(leftKid->leaf)) {
                        fix_child_size(leftKid, leftKid->size);
                        leftKid = leftKid->child[leftKid->size];
                    }
                    curr->key[i] = delete_Node(leftKid, leftKid->size - 1);
                }

                    // Replace with successor
                else if (rightKid->size >= min_degree) {
                    while (!(rightKid->leaf)) {
                        fix_child_size(rightKid, 0);
                        rightKid = rightKid->child[0];
                    }
                    curr->key[i] = delete_Node(rightKid, 0);
                }

                    // Merge children and move down the tree.
                else {
                    merge_Children(curr, i);
                    curr = leftKid;
                    continue;
                }
            }
            return toReturn;
        }

            // If the item has not been found, move down the tree.
        else {

            // If at a leaf, then the item isn't present.
            // Adjust curr and move down the tree.
            char result = findIndex(curr, i);
            if (result == '2') {
                curr = root;
            }
            else {
                curr = curr->child[findIndex(curr, k)];
            }
        }
    }
}

template <typename T>
std::pair<BNode<T>*, unsigned> BTree<T>::search(T element) {

    BNode<T> *x = root;
    while (true) {

        // Find the proper index in the current node's array.
        unsigned i = findIndex(x, element);

        if (i < x->size && !(less_than(element, x->key[i]) || less_than(x->key[i], element))) {
            return std::pair<BNode<T>*, unsigned>(x, i);
        }

            // Hit the bottom of the tree.
        else if (x->leaf) {
            return std::pair<BNode<T>*, unsigned>(NULL, 0);
        }

        else {
            x = x->child[i];
        }
    }
}

template <typename T>
T BTree<T>::search_key(T k) {
    std::pair<BNode<T>*, unsigned> node = search(k);

    if (node.first == NULL) {
        return -1;
    }
    return node.first->key[node.second];
}

template <typename T>
void BTree<T>::print_tree() {
    if (print_key != NULL && this->root != NULL) {
        printf("\n");

        print_Node(root, 0);
        printf("\n");
    }
}