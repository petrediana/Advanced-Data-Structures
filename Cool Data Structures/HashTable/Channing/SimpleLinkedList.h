#ifndef CHANNING_SIMPLELINKEDLIST_H
#define CHANNING_SIMPLELINKEDLIST_H

#include <string>
#include <vector>

class LinkedList {
public:
    LinkedList();
    LinkedList (const LinkedList &ls);
    ~LinkedList();
    LinkedList &operator=(const LinkedList &);
    bool insert(const std::string&);
    bool remove(const std::string&);
    bool search(const std::string&)const;
    std::vector<std::string> get() const;

private:
        class Node {
        public:
            std::string value;
            Node *next;

            Node(std::string value) : value(value), next(NULL) {};
            Node(std::string value, Node *next) : value(value), next(next) {};
        };

    Node *first;
    void copy_elements (const LinkedList &ls);
    void delete_elements();
};
#endif
