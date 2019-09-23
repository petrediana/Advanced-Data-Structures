#include <iostream>
#include "XORDLL.h"

Doubly_List ::Doubly_List() {
    this->size = 0;
    this->data = -1;
    this->head = nullptr;
    this->NPX = nullptr;
}

Doubly_List ::~Doubly_List() {
    if (head != nullptr) {
        Doubly_List *current_node = head;
        Doubly_List *prev = nullptr;
        Doubly_List *next;

        while (current_node != nullptr) {
            next = XOR(prev, current_node->NPX);
            prev = current_node;

            delete current_node;
            current_node = next;
        }
    }
}

Doubly_List* Doubly_List ::XOR(Doubly_List *element1, Doubly_List *element2) {
    return (Doubly_List*) ((uintptr_t) (element1) ^ (uintptr_t) (element2) );
}

int Doubly_List ::get_size() const {
    return size;
}

//insert at the beginning
void Doubly_List ::insert_element(int data_to_insert) {
    Doubly_List *new_node = new Doubly_List();
    new_node->data = data_to_insert;
    new_node->NPX = XOR(this->head, nullptr);

    if (this->head != nullptr) {
        Doubly_List *NEXT = XOR(this->head->NPX, nullptr);
        this->head->NPX = XOR(new_node, NEXT);
    }
    this->head = new_node;
    ++size;
}

void Doubly_List ::print_elements() {
    Doubly_List *current_node = this->head;
    Doubly_List *prev = nullptr;
    Doubly_List *next;

    std::cout << "There are " << get_size() << " elements.\n";
    while (current_node != nullptr) {
        std::cout << current_node->data << " ";

        //get the address of the next node!
        //current_node->NPX = next OR prev; which means that
        //prev OR current_node->NPX = next OR prev OR prev -> NEXT!
        next = XOR(prev, current_node->NPX);
        prev = current_node;
        current_node = next;
    }
}

int &Doubly_List::operator[](unsigned int index) {
    if (index > get_size())
        throw -1;
    else {
        Doubly_List *current_node = this->head;
        Doubly_List *prev = nullptr;
        Doubly_List *next;

        for (int i = 0; i < index; ++i) {
            next = XOR(prev, current_node->NPX);
            prev = current_node;
            current_node = next;
        }

        return current_node->data;
    }
}

void Doubly_List ::remove_element(int data_to_remove) {
    if (head != nullptr) {
        Doubly_List *current_node = this->head;
        Doubly_List *prev = nullptr;
        Doubly_List *next;

        while (current_node != nullptr ) {
            next = XOR(prev, current_node->NPX);

            if (current_node->data == data_to_remove) {
                std::cout << "\nFound! Removing: " << current_node->data << std::endl;

                Doubly_List *to_remove = current_node;
                current_node = next;
                delete to_remove;
                --size;


                return;
            }
            prev = current_node;
            current_node = next;
        }

    }
}