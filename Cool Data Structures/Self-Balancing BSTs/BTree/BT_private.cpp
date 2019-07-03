#include "BTree.h"
#include <bits/stdc++.h>
//implementation for the private methods

//initialize a B Tree node
template <typename T>
void BTree<T>::initialize_Node(BNode<T> *node) {
    node->size = 0;
    node->key = (T*)malloc(sizeof(T) * (2 * min_degree - 1));
    node->child = (BNode<T>**)malloc(sizeof(BNode<T>*) * 2 * min_degree);
}

//free the B Tree recursively
template <typename T>
void BTree<T>::free_Node(BNode<T> *node) {
    if (!node->leaf) {
        for (unsigned i = 0; i < node->size; ++i) {
            free_Node(node->child[i]);
        }
    }

    free(node->child);
    free(node->key);
    free(node);
}

template <typename T>
unsigned BTree<T>::findIndex(BNode<T> *x, T k) {
    unsigned i = 0;
    while (i < x->size && less_than(x->key[i], k)) {
        i++;
    }
    return i;
}

template <typename T>
unsigned BTree<T>::insert_Node(BNode<T> *node, T element) {
    //the new element needs to be somewhere between the existing elements
    //so we make room for it
    unsigned i;
    for (i = node->size; i > 0 && less_than(element, node->key[i - 1]); --i) {
        node->key[i] = node->key[i - 1];
        node->child[i + 1] = node->child[i];
    }

    //now insert the element
    node->child[i + 1] = node->child[i];
    node->key[i] = element;
    ++(node->size);

    return i;
}

template <typename T>
T BTree<T>::delete_Node(BNode<T> *node, unsigned index) {
    T deleted = node->key[index];

    --(node->size);
    while(index < node->size) {
        node->key[index] = node->key[index + 1];
        node->key[index + 1] = node->key[index + 2];
        ++index;
    }

    return deleted;
}

template <typename T>
void BTree<T>::split_Child(BNode<T> *node, int i) {
    //split the nodes that are full
    // i -> index of the node children that we need to split

    BNode<T> *split_me = node->child[i];
    BNode<T> *new_node = (BNode<T>*)malloc(sizeof(BNode<T>));
    initialize_Node(new_node);
    new_node->leaf = split_me->leaf;
    new_node->size = split_me->size - 1;

    //copy the second half of the node's keys
    for (unsigned j = 0; j < min_degree - 1; ++j) {
        new_node->key[j] = split_me->key[j + min_degree];
    }

    if (!split_me->leaf) {
        for (unsigned j = 0; j < min_degree - 1; ++j) {
            new_node->key[j] = split_me->key[j + min_degree];
        }
    }

    split_me->size = min_degree - 1;
    insert_Node(node, split_me->key[min_degree - 1]);
    node->child[i + 1] = new_node;
}

// Merges the (i + 1)th child of parent with the ith child of parent.
template <typename T>
char BTree<T>::merge_Children(BNode<T> *parent, unsigned i) {

    BNode<T> *left_Kid = parent->child[i];
    BNode<T> *right_Kid = parent->child[i + 1];

    // Move item from parent to left child.
    left_Kid->key[left_Kid->size] = delete_Node(parent, i);
    unsigned j = ++(left_Kid->size);

    // Move everything from rightKid into leftKid
    for (unsigned k = 0; k < right_Kid->size; k++) {
        left_Kid->key[j + k] = right_Kid->key[k];
        left_Kid->child[j + k] = right_Kid->child[k];
    }

    left_Kid->size += right_Kid->size;
    left_Kid->child[left_Kid->size] = right_Kid->child[right_Kid->size];

    // Free the memory used by rightChild
    free(right_Kid->child);
    free(right_Kid->key);
    free(right_Kid);

    // If parent is empty, than it must have been the root.
    if (parent->size == 0) {
        root = left_Kid;
        free(parent->child);
        free(parent->key);
        free(parent);
        return '2';
    }

    return '0';
}

// Makes sure parent->child[index] has at least minDegree items.
// If it doesn't, then things are changed to make sure it does.
template <typename T>
char BTree<T>::fix_child_size(BNode<T> *parent, unsigned index) {
    BNode<T> *kid = parent->child[index];

    if (kid->size < min_degree) {

        // Borrow from left sibling if possible.
        if (index != 0 && parent->child[index - 1]->size >= min_degree) {
            BNode<T> *leftKid = parent->child[index - 1];

            // When there are numerous equivalent keys,
            // nodeInsert can insert into an index other than 0.
            // The for loop fixed child pointers if that happens.

            for (unsigned i = insert_Node(kid, parent->key[index - 1]); i != 0; --i) {
                kid->child[i] = kid->child[i - 1];
            }

            kid->child[0] = leftKid->child[leftKid->size];
            parent->key[index - 1] = delete_Node(leftKid, leftKid->size - 1);
        }

            // Borrow from right sibling if possible
        else if (index != parent->size && parent->child[index + 1]->size >= min_degree) {

            BNode<T> *rightKid = parent->child[index + 1];

            // Move curr->key[i] into kid->key
            insert_Node(kid, parent->key[index]);
            kid->child[kid->size] = rightKid->child[0];
            rightKid->child[0] = rightKid->child[1];

            // Move rightKid->key[0] into curr->key
            parent->key[index] = delete_Node(rightKid, 0);
        }

            // If borrowing is not possible, then merge.
        else if (index != 0) {
            return merge_Children(parent, index - 1);
        }
        else {
            return merge_Children(parent, index);
        }
    }

    return '1';
}

template <typename T>
void BTree<T>::print_Node(BNode<T> *node, unsigned tab) {
    for (int i = 0; i < tab; ++i)
        printf("\t");

    //print the current node
    for (int i = 0; i < node->size; ++i) {
        print_key(node->key[i]);
        printf("  |  ");
    }

    //print the rest
    if (!node->leaf) {
        ++tab;
        for (int i = 0; i < node->size; ++i) {
            print_Node(node->child[i], tab);
        }
    }
}
