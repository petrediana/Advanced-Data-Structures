#ifndef XOR_DOUBLY_LINKED_LIST_XORDLL_H
#define XOR_DOUBLY_LINKED_LIST_XORDLL_H


class Doubly_List {
public:
    Doubly_List();
    void insert_element(int);
    void print_elements();
    void remove_element(int);
    ~Doubly_List();

    int &operator[](unsigned int);

private:
    int data;
    int size;
    Doubly_List *head;
    Doubly_List *NPX; //this holds the pointer for the XOR operation

    Doubly_List *XOR(Doubly_List*, Doubly_List*);
    int get_size() const;
};

#endif
