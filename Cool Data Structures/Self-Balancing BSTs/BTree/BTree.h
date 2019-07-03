//
// Created by Diana on 03-Jul-19.
//
#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#include <windef.h>
#include <utility>

template <typename T>
struct BNode {
    BNode<T> **child;	// Array of pointers to children.
    T *key;				// Array of keys.
    unsigned size;		// Number of keys.
    bool leaf;			// Whether the node is a leaf.
};


template <typename T>
class BTree {
public:
    BTree(unsigned, bool(*)(T, T), void (*)(T) = NULL);
    ~BTree();
    void insert(T);
    T remove(T);
    std::pair<BNode<T>*, unsigned> search(T);
    T search_key(T);
    void print_tree();

private:
    void initialize_Node(BNode<T>*);
    void free_Node(BNode<T>*);
    unsigned findIndex(BNode<T>*, T);
    unsigned insert_Node(BNode<T>*, T);
    T delete_Node(BNode<T>*, unsigned);
    void split_Child(BNode<T>*, int);
    char merge_Children(BNode<T>*, unsigned);
    char fix_child_size(BNode<T>*, unsigned);
    void print_Node(BNode<T>*, unsigned);

    bool (*less_than)(T, T);
    void (*print_key)(T);

    BNode<T> *root;
    unsigned min_degree;
};

#endif //BTREE_BTREE_H

/*
 * Constructor: unsigned -> min degree of the tree;
 *              bool(*) -> key comparison function;
 *              void(*) -> a function that will print the keys
 *
 * pair<> search: -> finds a key in the tree
 *
 * */