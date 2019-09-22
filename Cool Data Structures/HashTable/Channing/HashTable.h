#ifndef CHANNING_HASHTABLE_H
#define CHANNING_HASHTABLE_H

#include "SimpleLinkedList.h"

class HashTable {
public:
    HashTable();
    HashTable(int);
    HashTable(const HashTable&);
    ~HashTable();
    HashTable &operator=(const HashTable&);
    bool insert(const std::string&);
    bool search(const std::string&) const;
    bool remove(const std::string&);
    int size() const;
    int max_size() const;
    double loadFactor() const;

private:
    LinkedList *elements;
    int length;
    int no_of_elements;

    int hash_function(const std::string&) const;
    void copy_hash(const HashTable &);
    std::vector<std::string> get_info() const;
};
#endif
