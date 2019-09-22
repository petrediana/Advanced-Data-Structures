#include "HashTable.h"

HashTable ::HashTable() {
    length = 101;
    elements = new LinkedList[length];
    no_of_elements = 0;
}
HashTable ::HashTable(const HashTable &ht) {
    copy_hash(ht);
}

HashTable ::~HashTable() {
    delete[] elements;
}

HashTable& HashTable ::operator=(const HashTable &ht) {
    if (this != &ht) {
        if (ht.elements != NULL)
            delete[] elements;
        copy_hash(ht);
    }

    return *this;
}

int HashTable ::hash_function(const std::string &str) const {
    int hash = 0, asc;

    for (int i = 0; i < str.size(); ++i) {
        asc = str[i] > 96 ? str[i] - 96 : str[i] - 64;
        hash = (hash * 32 + asc) % length;
    }

    return hash;
}

void HashTable ::copy_hash(const HashTable &ht) {
    if (ht.elements != NULL) {
        no_of_elements = ht.size();
        length = ht.max_size();

        elements = new LinkedList[length];
        for (int i = 0; i < length; ++i)
            elements[i] = ht.elements[i];
        // = operator for linkedlist!!
    }
}

std::vector<std::string> HashTable ::get_info() const {
    std::vector<std::string> hash_info, list_info;

    for (int i = 0; i < max_size(); ++i) {
        list_info = elements[i].get();
            for (int j = 0; j < list_info.size(); ++j)
                hash_info.emplace_back(list_info[j]);
    }

    return hash_info;
}

bool HashTable ::insert(const std::string &given_values) {
    int hash = hash_function(given_values);
    bool result = elements[hash].insert(given_values);
    ++no_of_elements;

    return result;
}

bool HashTable ::remove(const std::string &given_values) {
    int hash = hash_function(given_values);
    bool result = elements[hash].remove(given_values);
    --no_of_elements;

    return result;
}

bool HashTable ::search(const std::string &given_values) const {
    int hash = hash_function(given_values);
    bool result = elements[hash].search(given_values);

    return result;
}

int HashTable ::size() const {
    return no_of_elements;
}

int HashTable ::max_size() const {
    return length;
}

double HashTable ::loadFactor() const {
    return (no_of_elements * 1.00) / length;
}
