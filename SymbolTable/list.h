/**
* File: list.h
* ------------
* Simple list class for storing a linear collection of elements.
* It supports operations: getNth, insert, append, remove, etc.
*/

#ifndef LIST_H__
#define LIST_H__

#include <stdio.h>
#include <deque>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>
#include <string.h>
#include <vector>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

class Node;

template<class Element>
class List {
private:
    std::deque<Element> elems;

public:
    // Create a new empty list
    List() {}

    // Returns count of elements currently in list
    int 
    getNumElements() const 
    {
        return elems.size();
    }

    // Returns element at index in list. Indexing is 0-based.
    // Raises an assert if index is out of range.
    Element
    getNth(int index) const 
    {
        assert(index >= 0);
        assert(index < getNumElements());
        return elems[index];
    }

    // Inserts element at index, shuffling over others
    // Raises assert if index out of range
    void 
    insertAt(const Element &elem, int index) 
    {
        assert(index >= 0 && index <= getNumElements());
        elems.insert(elems.begin() + index, elem);
    }

    // Adds element to list end
    void 
    append(const Element &elem)
    {
        elems.push_back(elem);
    }

    // Removes element at index, shuffling down others
    // Raises assert if index out of range
    void 
    removeAt(int index)
    {
        assert(index >= 0 && index < getNumElements());
        elems.erase(elems.begin() + index);
    }

    // These are some specific methods useful for lists of ast nodes
    // They will only work on lists of elements that respond to the
    // messages, but since C++ only instantiates the template if you use
    // you can still have Lists of ints, chars*, as long as you
    // don't try to setParentAll on that list.
    void 
    setParentAll(Node *p) 
    {
        for (int i = 0; i < getNumElements(); i++)
        {
            getNth(i)->set_parent(p);
        }
    }

    void
    printAll(int indentLevel, const char *label = NULL)
    {
        for (int i = 0; i < getNumElements(); i++) 
        {
            getNth(i)->print(indentLevel, label);
        }
    }

};
#endif
