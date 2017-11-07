#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include "priority_queue.h"
#include <list>

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a 
//           Fibonacci heap.
template<typename TYPE,typename COMP = std::less<TYPE>>
class fib_heap:public priority_queue<TYPE,COMP>
{
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    fib_heap(COMP comp=COMP());

    // EFFECTS: Deconstruct the heap with no memory leak.
    // MODIFIES: this
    // RUNTIME: O(n)
    ~fib_heap();
    
    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE&val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: Amortized O(log(n))
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(1)
    virtual const TYPE&get_min() const;

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
    // You may want to define a strcut/class to represent nodes in the heap and a
    // pointer to the min node in the heap.
    struct fib_node
    {
        TYPE val;
        typename std::list<fib_node> child;
        int degree=0;
    };
    typename std::list<fib_node> root;
    typename std::list<fib_node>::iterator H_min;
    int H_n=0;
    TYPE empty_element=TYPE();
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

template<typename TYPE,typename COMP>
fib_heap<TYPE,COMP>::fib_heap(COMP comp)
{
    compare=comp;
    // Fill in the remaining lines if you need.
    H_min=root.begin();
    H_n=0;
}

template<typename TYPE,typename COMP>
fib_heap<TYPE,COMP>::~fib_heap()
{
    typename std::list<fib_node>::iterator it;
    for(it=root.begin();it!=root.end();++it)
    {
        root.erase(it);
    }
}

template<typename TYPE,typename COMP>
void fib_heap<TYPE,COMP>::enqueue(const TYPE&val)
{
    fib_node n;
    n.val=val;
    n.degree=0;
    if(root.empty()==true)
    {
        root.push_back(n);
        H_min=root.begin();
    }
    else
    {
        if(compare(val,(*H_min).val))
        {
            auto it=root.insert(root.end(),n);
            H_min=it;
        }
        else
        {
            root.insert(root.end(),n);
        }
    }
    H_n++;
};

template<typename TYPE,typename COMP>
TYPE fib_heap<TYPE,COMP>::dequeue_min()
{
    if(root.empty()==true)
    {
        return empty_element;
    }
    
//    std::cout<<std::endl;
    
    fib_node z;
    z=*H_min;
    typename std::list<fib_node>::iterator temp;
    if(H_min!=root.end())
    {
        temp=z.child.begin();
        while(temp!=z.child.end())
        {
            root.push_back(*temp);
            temp=z.child.erase(temp);
        }
        H_min=root.erase(H_min);
        H_n--;
        if(H_n==0)
        {
            H_min=root.end();
        }
        else
        {
            int size=int((log(H_n))/(log((1+sqrt(5))/2)))+1;
            typename std::list<fib_node>::iterator A[size];
            for(int i=0;i<size;++i)
            {
                A[i]=root.end();
            }
            typename std::list<fib_node>::iterator x;
            typename std::list<fib_node>::iterator y;
            int d=0;
            typename std::list<fib_node>::iterator it;
            for(it=root.begin();it!=root.end();++it)
            {
                
//                std::cout<<"item in A ";
//                for(int i=0;i<size;++i)
//                {
//                    if(A[i]==root.end()){
//                        std::cout<<"NULL"<<" ";
//                    }
//                    else{
//                        std::cout<<(*A[i]).val<<" ";
//                    }
//                }
//                std::cout<<std::endl;
                
                d=(*it).degree;
                while(A[d]!=root.end())
                {
                    y=A[d];
                    
//                    std::cout<<"item in A ";
//                    for(int i=0;i<size;++i)
//                    {
//                        if(A[i]==root.end()){
//                            std::cout<<"NULL"<<" ";
//                        }
//                        else{
//                            std::cout<<(*A[i]).val<<" ";
//                        }
//                    }
//                    std::cout<<std::endl;
                    
                    if(compare((*y).val,(*it).val))
                    {
                        root.insert(y,*it);
                        root.insert(it,*y);
                        it=root.erase(it);
                        y=root.erase(y);
                        it--;
                        y--;
                    }
                    
//                    std::cout<<"item in A ";
//                    for(int i=0;i<size;++i)
//                    {
//                        if(A[i]==root.end()){
//                            std::cout<<"NULL"<<" ";
//                        }
//                        else{
//                            std::cout<<(*A[i]).val<<" ";
//                        }
//                    }
//                    std::cout<<std::endl;
                    
                    (*it).child.push_back((*y));
                    y=root.erase(y);
                    (*it).degree++;
                    A[d]=root.end();
                    
//                    std::cout<<"item in A ";
//                    for(int i=0;i<size;++i)
//                    {
//                        if(A[i]==root.end()){
//                            std::cout<<"NULL"<<" ";
//                        }
//                        else{
//                            std::cout<<(*A[i]).val<<" ";
//                        }
//                    }
//                    std::cout<<std::endl;
//                    typename std::list<fib_node>::iterator ttt;
//                    int testt=0;
//                    for(ttt=root.begin();ttt!=root.end();++ttt)
//                    {
//                        printf("The %d item in rootlost is %d\n",testt,(*ttt).val);
//                        typename std::list<fib_node>::iterator tttt;
//                        for(tttt=(*ttt).child.begin();tttt!=(*ttt).child.end();++tttt)
//                        {
//                            std::cout<<(*tttt).val<<" ";
//                        }
//                        testt++;
//                        std::cout<<std::endl;
//                    }
//                    std::cout<<"END"<<std::endl;
                    
                    d++;
                }
                A[d]=it;
            }
            H_min=root.end();
            for(int i=0;i<size;++i)
            {
                if(A[i]!=root.end())
                {
                    if(H_min==root.end())
                    {
                        H_min=A[i];
                    }
                    else
                    {
                        if(compare((*A[i]).val,(*H_min).val))
                        {
                            H_min=A[i];
                        }
                    }
                }
            }
        }
    }
    return z.val;
};

template<typename TYPE,typename COMP>
const TYPE&fib_heap<TYPE,COMP>::get_min() const
{
    if(this->empty())
    {
        return empty_element;
    }
    else
    {
        return (*H_min).val;
    }
};

template<typename TYPE,typename COMP>
bool fib_heap<TYPE,COMP>::empty() const
{
    return this->size()==0;
};

template<typename TYPE,typename COMP>
unsigned fib_heap<TYPE,COMP>::size() const
{
    return this->H_n;
};

#endif //FIB_HEAP_H
