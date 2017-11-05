//
// Created by 大泽 on 2017/11/5.
//

#ifndef PROJECT3_FIB_HEAP2_H
#define PROJECT3_FIB_HEAP2_H

#include <algorithm>
#include <cmath>
#include <list>
#include "priority_queue.h"

using namespace std;

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a
//           Fibonacci heap.
template <typename TYPE, typename COMP = std::less<TYPE> >
class fib_heap : public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    fib_heap(COMP comp = COMP());

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE &val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: Amortized O(log(n))
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(1)
    virtual const TYPE &get_min() const;

    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const;

    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const;

private:
    // Note: compare is a functor object
    COMP compare;

private:
    struct node {
        TYPE key = TYPE();
        node *parent = NULL;
        std::list<node> children_list;
// Initial intention to implement it with a list later to find not so
// intuitive as directly use pointers.

        unsigned int degree = 0;
    };
    unsigned int n = 0; // Number of elements
//    node *min_node;
    typename std::list<node>::iterator min_node;
//    std::list<node> root_list;
    typename std::list<node> root_list;

    virtual void consolidate();

    virtual void Fib_Heap_Link(node y, node x);
};


template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::fib_heap(COMP comp) {
    n = 0;
    compare = comp;
    min_node = root_list.end();
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue(const TYPE &val) {
    auto *new_node = new node;
    new_node->key = val;
    if (root_list.empty()) {
        root_list.push_back(*new_node);
        typename std::list<node>::iterator x;
        x = root_list.end();
        min_node = x--;
    }
    else {
        root_list.push_back(*new_node);
        if (compare(new_node->key, min_node->key)) {
            typename std::list<node>::iterator x;
            x = root_list.end();
            min_node = x--;
        }
        this->n++;
    }
}


template <typename TYPE, typename COMP>
const TYPE &fib_heap<TYPE, COMP>::get_min() const {
    return (*min_node).key;
}

template <typename TYPE, typename COMP>
unsigned int fib_heap<TYPE, COMP>::size() const {
    return this->n;
}

template <typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP>::empty() const {
    return this->size() == 0;
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min() {
    typename std::list<node>::iterator z = this->min_node;
    if (z != root_list.end()) {
        typename std::list<node>::iterator x;
        for (x = (*z).children_list.begin(); x != (*z).children_list.end(); x++) {
            root_list.push_back(*x);
            (*x).parent = NULL;
            (*z).children_list.erase(x);
        }
        if (root_list.size() == 1) {
            this->min_node = root_list.end();
        }
        else {
            auto s = z++;
            this->min_node = s;
            consolidate();
        }
        n--;
    }
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::Fib_Heap_Link(node y, node x) {
    auto *temp1 = &y;
    typename std::list<node>::iterator temp;
    *temp = y;
    root_list.erase(temp);
    x.children_list.push_back(*temp1);
    *temp->parent = x;
    x.degree++;
    //y.mark=false
}


template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate() {
    int size_of_A;
    auto n0 = n;
    double temp = log(n0) / log(1.618);
    size_of_A = static_cast<int>(floor(temp) + 1);
    typename std::list<node>::iterator A[size_of_A];
    int i;
    for (i = 0; i <= size_of_A; i++) {
        A[i] = root_list.end();
    }
    typename std::list<node>::iterator it;
    for (it = root_list.begin(); it != root_list.end(); it++) {
        node x = *it;
        auto d = x.degree;
        while (A[d] != root_list.end()) {
            auto y = *A[d];
            if (compare(x.key, y.key)) {
                Fib_Heap_Link(x, y);
            }
            else {
                Fib_Heap_Link(y, x);
            }
            A[d] = root_list.end();
            d++;
        }
        *(A[d]) = x;
    }
    this->min_node = root_list.end();
    for (i = 0; i <= size_of_A; i++) {
        if (A[i] != root_list.end()) {
            if (min_node == root_list.end()) {
                root_list.push_back(*A[i]);
                min_node = A[i];
            }
            else {
                if (compare((*min_node).key, (*A[i]).key)) {
                    min_node = A[i];
                }
            }
        }
    }
}

#endif //PROJECT3_FIB_HEAP2_H
