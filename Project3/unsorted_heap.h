#ifndef UNSORTED_HEAP_H
#define UNSORTED_HEAP_H

#include <algorithm>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT that is implemented with
//           an underlying unordered array-based container. Every time a min
//           is required, a linear search is performed.
template <typename TYPE, typename COMP = std::less<TYPE> >
class unsorted_heap : public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    unsorted_heap(COMP comp = COMP());

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE &val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: O(n)
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(n)
    virtual const TYPE &get_min() const;

    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const;

    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const;

private:
    // Note: This vector *must* be used in your heap implementation.
    std::vector<TYPE> data;
    // Note: compare is a functor object
    COMP compare;
private:
    // Add any additional member functions or data you require here.
};

template <typename TYPE, typename COMP>
unsorted_heap<TYPE, COMP>::unsorted_heap(COMP comp) {
    compare = comp;
    // Fill in the remaining lines if you need.
    data.push_back(TYPE());
}

template <typename TYPE, typename COMP>
void unsorted_heap<TYPE, COMP>::enqueue(const TYPE &val) {
    // Fill in the body.
    data.push_back(val);
}

template <typename TYPE, typename COMP>
TYPE unsorted_heap<TYPE, COMP>::dequeue_min() {
    // Fill in the body.
    auto temp = data[1];
    auto a = 0;
    for (size_type i = 1; i <= data.size() - 1; i++) {
        if (!compare(data[i], temp)) {
            temp = data[i];
            a = i;
        }
    }
    TYPE val = temp;
    data.erase(data.begin() + a);
    return val;
}

template <typename TYPE, typename COMP>
const TYPE &unsorted_heap<TYPE, COMP>::get_min() const {
    // Fill in the body.
    auto temp = data[1];
    for (size_type i = 1; i <= data.size() - 1; i++) {
        if (!compare(data[i], temp)) {
            temp = data[i];
        }
    }
    TYPE val = temp;
    return val;
}

template <typename TYPE, typename COMP>
bool unsorted_heap<TYPE, COMP>::empty() const {
    // Fill in the body.
    return this->size() == 0;
}

template <typename TYPE, typename COMP>
unsigned unsorted_heap<TYPE, COMP>::size() const {
    // Fill in the body.
    auto val = data.size() - 1;
    return val;
}

#endif //UNSORTED_HEAP_H
