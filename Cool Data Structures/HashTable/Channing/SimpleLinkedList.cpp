#include "SimpleLinkedList.h"

LinkedList::LinkedList() {
    first = NULL;
}

LinkedList::LinkedList(const LinkedList &ls) {
    copy_elements(ls);
}

LinkedList::~LinkedList() {
    delete_elements();
}

LinkedList &LinkedList::operator=(const LinkedList &ls) {
    if (this != &ls) {
        delete_elements();
        copy_elements(ls);
    }

    return *this;
}

void LinkedList::delete_elements() {
    Node *aux;

    while (first != NULL) {
        aux = first;
        first = first -> next;

        delete aux;
    }
}

void LinkedList::copy_elements(const LinkedList &ls) {
    if (ls.first != NULL) {
        if (this->first != NULL)
            delete_elements();

        first = NULL;
        Node *new_node = new Node(ls.first->value, NULL);
        first = new_node;

        Node *copy_of_ls = ls.first;
        copy_of_ls = copy_of_ls->next;

        while (copy_of_ls != NULL) {
            new_node->next = new Node(copy_of_ls->value, NULL);
            new_node = new_node->next;
            copy_of_ls = copy_of_ls->next;
        }
        //first will be updated with the new_node
    }
}

std::vector<std::string> LinkedList::get() const {
    std::vector<std::string> get_values;

    Node *aux = first;

    while (aux != NULL) {
        get_values.emplace_back(aux->value);
        aux = aux ->next;
    }

    return get_values;
}

bool LinkedList ::search(const std::string &given_value) const {
    Node *aux = first;

    while (aux != NULL) {
        if (aux->value == given_value)
            return true;

        aux = aux->next;
    }

    return false;
}

bool LinkedList ::remove(const std::string &given_value) {
    if (this->first == NULL)
        return false;

    Node *aux = first;

    //1. Check if the given_value is the first element
    if (aux->value == given_value) {
        first = first->next;
        delete aux;

        return true;
    } else {
        //2. The value is not in the first Node, check for the rest Nodes
        while (aux->next!= NULL) {
            if (aux->next->value == given_value) {
                Node *aux2 = aux->next;
                aux->next = aux->next->next;
                delete aux2;

                return true;
            }

            aux = aux->next;
        }

        return false;
    }
}

bool LinkedList ::insert(const std::string &given_value) {
    if (search(given_value))
        return false; // the value already exists in the LL
    else {
        this->first = new Node(given_value, first);
        return true;
    }
}
