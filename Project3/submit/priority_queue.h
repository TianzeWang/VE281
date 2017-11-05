#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <functional>
#include <vector>

// OVERVIEW: A simple interface that implements a generic heap.
//           Runtime specifications assume constant time comparison and
//           copying. TYPE is the type of the elements stored in the priority
//           queue. COMP is a functor, which returns the comparison result of
//           two elements of the type TYPE. See test_heap.cpp for more details
//           on functor.
template<typename TYPE, typename COMP = std::less<TYPE> >
class priority_queue {
public:
    typedef unsigned size_type;

    virtual ~priority_queue() {}

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(n) - some implementations *must* have tighter bounds (see
    //          specialized headers).
    virtual void enqueue(const TYPE &val) = 0;

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    //           Note: We will not run tests on your code that would require it
    //           to dequeue an element when the heap is empty.
    // MODIFIES: this
    // RUNTIME: O(n) - some implementations *must* have tighter bounds (see
    //          specialized headers).
    virtual TYPE dequeue_min() = 0;

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(n) - some implementations *must* have tighter bounds (see
    //          specialized headers).
    virtual const TYPE &get_min() const = 0;

    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const = 0;

    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const = 0;

};

#endif //PRIORITY_QUEUE_H
