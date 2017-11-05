#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <algorithm>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a binary
//           heap.
template <typename TYPE, typename COMP = std::less<TYPE> >
class binary_heap : public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    binary_heap(COMP comp = COMP());

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(log(n))
    virtual void enqueue(const TYPE &val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: O(log(n))
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
    // Note: This vector *must* be used in your heap implementation.
    std::vector<TYPE> data;
    // Note: compare is a functor object
    COMP compare;
private:
    // Add any additional member functions or data you require here.

    // EFFECTS: Sort the heap according to the specified COMP
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: O(n)
    virtual void sort();
};

template <typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp) {
    compare = comp;
    // Fill in the remaining lines if you need.
    data.push_back(TYPE());
}

template <typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::enqueue(const TYPE &val) {
    // Fill in the body.
    data.push_back(val);
    this->sort();
}

template <typename TYPE, typename COMP>
TYPE binary_heap<TYPE, COMP>::dequeue_min() {
    // Fill in the body.
    std::swap(data[1], data[data.size() - 1]);
    auto temp = data[data.size() - 1];
    data.pop_back();
    this->sort();
    return temp;
}

template <typename TYPE, typename COMP>
const TYPE &binary_heap<TYPE, COMP>::get_min() const {
    // Fill in the body.
    return data[1];
}

template <typename TYPE, typename COMP>
bool binary_heap<TYPE, COMP>::empty() const {
    // Fill in the body.
    return this->size() == 0;
}

template <typename TYPE, typename COMP>
unsigned binary_heap<TYPE, COMP>::size() const {
    // Fill in the body.
    auto val = data.size() - 1;
    return static_cast<unsigned int>(val);
}

template <typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::sort() {
    size_type temp = (data.size() - 1) / 2;
    for (size_type i = temp; i >= 1; i--) {
        auto j = i;
        while (2 * j <= data.size() - 1) {
            if (2 * j + 1 > data.size() - 1) {
                if (!compare(data[j], data[2 * j])) {
                    std::swap(data[j], data[2 * j]);
                    j = 2 * j;
                }
                else break;
            }
            else {
                if (!compare(data[j], data[2 * j]) && compare(data[2 * j], data[2 * j + 1])) {
                    std::swap(data[j], data[2 * j]);
                    j = 2 * j;
                }
                else if (!compare(data[j], data[2 * j + 1]) && compare(data[2 * j + 1], data[2 * j])) {
                    std::swap(data[j], data[2 * j + 1]);
                    j = 2 * j + 1;
                }
                else break;
            }
        }
    }
}

#endif //BINARY_HEAP_H
