#ifndef UNSORTED_HEAP_H
#define UNSORTED_HEAP_H

#include <algorithm>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT that is implemented with
//           an underlying unordered array-based container. Every time a min
//           is required, a linear search is performed.
template<typename TYPE,typename COMP = std::less<TYPE>>
class unsorted_heap:public priority_queue<TYPE,COMP>
{
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    unsorted_heap(COMP comp=COMP());

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE&val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: O(n)
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(n)
    virtual const TYPE&get_min() const;

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
    TYPE empty_element=TYPE();
};

template<typename TYPE,typename COMP>
unsorted_heap<TYPE,COMP>::unsorted_heap(COMP comp)
{
    compare=comp;
    // Fill in the remaining lines if you need.
}

template<typename TYPE,typename COMP>
void unsorted_heap<TYPE,COMP>::enqueue(const TYPE&val)
{
    // Fill in the body.
    data.push_back(val);
}

template<typename TYPE,typename COMP>
TYPE unsorted_heap<TYPE,COMP>::dequeue_min()
{
    // Fill in the body.
    if(this->empty())
    {
        return empty_element;
    }
    auto it=data.begin();
    TYPE min=data[0];
    TYPE max=data[0];
    for(it=data.begin();it!=data.end();++it)
    {
        if(compare((*it),min))
        {
            min=*it;
        }
    }
    for(it=data.begin();it!=data.end();++it)
    {
        if(compare(max,(*it)))
        {
            max=(*it);
        }
    }
    if(compare(min,max)==false)
    {
        min=max;
    }
    auto key=it;
    for(it=data.begin();it!=data.end();++it)
    {
        if((*it)==min)
        {
            key=it;
            break;
        }
    }
    data.erase(key);
    return min;
}

template<typename TYPE,typename COMP>
const TYPE&unsorted_heap<TYPE,COMP>::get_min() const
{
    // Fill in the body.
    if(this->empty())
    {
        return data[0];
    }
    auto it=data.begin();
    TYPE min=data[0];
    TYPE max=data[0];
    for(it=data.begin();it!=data.end();++it)
    {
        if(compare((*it),min))
        {
            min=(*it);
        }
    }
    for(it=data.begin();it!=data.end();++it)
    {
        if(compare(max,(*it)))
        {
            max=(*it);
        }
    }
    if(compare(min,max)==false)
    {
        min=max;
    }
    auto key=it;
    for(it=data.begin();it!=data.end();++it)
    {
        if((*it)==min)
        {
            key=it;
            break;
        }
    }
    return *key;
}

template<typename TYPE,typename COMP>
bool unsorted_heap<TYPE,COMP>::empty() const
{
    // Fill in the body.
    return data.empty();
}

template<typename TYPE,typename COMP>
unsigned unsorted_heap<TYPE,COMP>::size() const
{
    // Fill in the body.
    return data.size();
}

#endif //UNSORTED_HEAP_H
