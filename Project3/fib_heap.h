#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include "priority_queue.h"

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
    // Add any additional member functions or data you require here.
    struct node {
        TYPE key=TYPE();
        node *parent=NULL;
        node *child=NULL;
        size_type degree=0;
    };
    // You may want to define a strcut/class to represent nodes in the heap and a
    // pointer to the min node in the heap.

};

template <typename TYPE, typename COMP>
fib_heap::fib_heap(COMP comp) {
    compare = comp;

}

template <typename TYPE, typename COMP>
void fib_heap::enqueue(const TYPE &val) {

}

template <typename TYPE, typename COMP>
TYPE fib_heap::dequeue_min() {
    return nullptr;
}

template <typename TYPE, typename COMP>
const TYPE &fib_heap::get_min() const {
    return <#initializer#>;
}

template <typename TYPE, typename COMP>
fib_heap::size_type fib_heap::size() const {
    return 0;
}


template <typename TYPE, typename COMP>
bool fib_heap::empty() const {
    return false;
}




// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

#endif //FIB_HEAP_H
