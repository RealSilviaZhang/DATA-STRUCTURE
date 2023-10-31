/**
 * Assignment 3 for COSE213 Data Structures
 *
 * Won-Ki Jeong (wkjeong@korea.ac.kr)
 *
 * 2022. 5. 1
 *
 */
#include <algorithm>
#include <vector>

// destructor
template <class HashMapElemType> HashMap<HashMapElemType>::~HashMap() {
    HashMapElemType *cur, *next;
    for (int i = 0; i < divisor; i++) {
        // free the bucket
        if (ht[i] != nullptr) {
            cur = ht[i];
            // free the chain
            while (cur != nullptr) {
                next = cur->link;
                delete cur;
                cur = next;
            }
        }
    }
    delete[] ht;
}

template <class HashMapElemType>
HashMapElemType *HashMap<HashMapElemType>::find(const KeyType k) {
    int index = hashfunction(k);
    HashMapElemType *cur;
    if (ht[index] != nullptr) {
        // locate the bucket
        cur = ht[index];
        while (cur != nullptr) {
            if (cur->key == k) {
                // found
                return cur;
            }
            cur = cur->link;
        }
    }
    // not found
    return nullptr;
}

template <class HashMapElemType>
void HashMap<HashMapElemType>::insert(const KeyType k, const ValType v) {
    HashMapElemType *new_elem = new HashMapElemType;
    new_elem->key = k;
    new_elem->val = v;
    new_elem->link = nullptr;

    int index = hashfunction(k);
    if (ht[index] == nullptr) { // the first insert into this bucket
        ht[index] = new_elem;
        return;
    }

    HashMapElemType *pre = ht[index];
    HashMapElemType *cur = ht[index];
    while (cur != nullptr) {
        if (pre->key == k) {
            // duplicate!
            pre->val = v;
            return;
        }
        pre = cur;
        cur = cur->link;
    }
    // insert to tail
    pre->link = new_elem;
    mapsize++;
}

template <class HashMapElemType>
bool HashMap<HashMapElemType>::remove(const KeyType k) {
    int index = hashfunction(k);
    if (ht[index] == nullptr) {
        // not found
        return false;
    }

    HashMapElemType *pre = nullptr;
    HashMapElemType *cur = ht[index];
    while (cur != nullptr) {
        if (cur->key == k) {
            // found;
            if (pre == nullptr) { // the head
                ht[index] = cur->link;
            } else {
                pre->link = cur->link;
            }
            delete cur;
            mapsize--;
            return true;
        }
        pre = cur;
        cur = cur->link;
    }
    return false;
}

template <class HashMapElemType>
unsigned int HashMap<HashMapElemType>::hashfunction(const KeyType k) {
    // Polynomial Hash Code
    int hash_val = 0;
    int p = 31; // set exponent to 31
    int p_pow = 1;
    for (auto &ch : k) {
        hash_val = (hash_val + static_cast<int>(ch) * p_pow) % divisor;
        p_pow = (p_pow * p) % divisor;
    }
    return hash_val;
}

template <class HashMapElemType> void HashMap<HashMapElemType>::print() {
    // using sort in standard library
    std::vector<HashMapElemType *> hold;
    HashMapElemType *cur;
    for (int i = 0; i < divisor; i++) {
        cur = ht[i];
        while (cur != nullptr) {
            hold.push_back(cur);
            cur = cur->link;
        }
    }

    auto func = [&](const HashMapElemType *a, const HashMapElemType *b) {
        if (a->val != b->val) {
            return a->val > b->val;
        }
        return a->key < b->key;
    };

    // sorting
    std::sort(hold.begin(), hold.end(), func);

    // print
    for (auto ptr : hold) {
        std::cout << ptr->key << ":" << ptr->val << '\n';
    }
    std::cout << std::flush;
}