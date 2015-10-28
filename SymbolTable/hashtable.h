/* File: hashtable.h
* -----------------
* This is a simple table for storing values associated with a string
* key, supporting simple operations for enter and lookup.
*
* keys: strings
* values: any type
*/
#ifndef _H_hashtable
#define _H_hashtable
#include <map>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string.h>     // strdup
#include <utility>      // std::pair

template <class Value>
class Iterator;

template<class Value>

class Hashtable {
private:
    std::multimap<std::string, Value> mmap;

public:
    // ctor creates a new empty hashtable
    Hashtable() {}

    // Returns number of entries currently in table
    int getNumEntries() const;

    // Associates value with key. If a previous entry for
    // key exists, the bool parameter controls whether
    // new value overwrites the previous (removing it from
    // from the table entirely) or just shadows it (keeps previous
    // and adds additional entry). The lastmost entered one for an
    // key will be the one returned by Lookup.
    void enter(std::string key, Value value, bool overwriteInsteadOfShadow = true);

    // Removes a given key->value pair.  Any other values
    // for that key are not affected. If this is the last
    // remaining value for that key, the key is removed
    // entirely.
    void remove(std::string key, Value value);

    // Returns value stored under key or NULL if no match.
    // If more than one value for key (ie shadow feature was
    // used during Enter), returns the lastmost entered one.
    Value lookup(std::string key);

    void printall(void);

    // Returns an Iterator object (see below) that can be used to
    // visit each value in the table in alphabetical order.
    Iterator<Value> getIterator();
};

/*
 * Iteration implementation.
 */
template <class Value>
class Iterator {
    friend class Hashtable<Value>;
private:
    typename std::multimap<std::string, Value>::iterator cur, end;
    Iterator(std::multimap<std::string, Value>& t)
        : cur(t.begin()), end(t.end()) {
    }

public:
    // Returns current value and advances iterator to next.
    // Returns NULL when there are no more values in table
    // Visits every value, even those that are shadowed.
    Value getNextValue();
};

/* Hashtable::enter
* ----------------
* Stores new value for given identifier. If the key already
* has an entry and flag is to overwrite, will remove previous entry first,
* otherwise it just adds another entry under same key. Copies the
* key, so you don't have to worry about its allocation.
*/
template <class Value>
void Hashtable<Value>::enter(std::string key, Value val, bool overwrite) {
    assert(val);
    Value prev;

    if (overwrite && (prev = lookup(key))) {
        remove(key, prev);
    }
    mmap.insert(std::make_pair(key, val));
}

/* Hashtable::Remove
* -----------------
* Removes a given key-value pair from table. If no such pair, no
* changes are made.  Does not affect any other entries under that key.
*/
template <class Value>
    void Hashtable<Value>::remove(std::string key, Value val) {
        if (mmap.count(key) == 0) { // no matches at all
            return;
        }

        typename std::multimap<std::string, Value>::iterator itr;
        itr = mmap.find(key); // start at first occurrence
        while (itr != mmap.upper_bound(key)) {
            if (itr->second == val) { // iterate to find matching pair
                mmap.erase(itr);
                break;
            }
            ++itr;
        }
}

/* Hashtable::Lookup
* -----------------
* Returns the value earlier stored under key or NULL
* if there is no matching entry
*/
template <class Value>
Value Hashtable<Value>::lookup(std::string key) {
    Value found = NULL;

    if (mmap.count(key) > 0) {
        typename std::multimap<std::string, Value>::iterator cur, last, prev;
        cur = mmap.find(key); // start at first occurrence
        last = mmap.upper_bound(key);
        while (cur != last) { // iterate to find last entered
            prev = cur;
            if (++cur == mmap.upper_bound(key)) { // have to go one too far
                found = prev->second; // one before last was it
                break;
            }
        }
    }
    return found;
}

/* Hashtable::NumEntries
* ---------------------
*/
template <class Value>
int Hashtable<Value>::getNumEntries() const {
    return mmap.size();
}

/* Hashtable:GetIterator
* ---------------------
* Returns iterator which can be used to walk through all values in table.
*/
template <class Value>
Iterator<Value> Hashtable<Value>::getIterator() {
    return Iterator<Value>(mmap);
}

/* Iterator::GetNextValue
* ----------------------
* Iterator method used to return current value and advance iterator
* to next entry. Returns null if no more values exist.
*/
template <class Value>
Value Iterator<Value>::getNextValue() {
    if (cur != end) {
        return (*cur++).second;
    }
    return NULL;
}

template <class Value>
void Hashtable<Value>::printall(void) {
    typename std::multimap<std::string, Value>::const_iterator cur;

    for (cur = mmap.begin(); cur != mmap.end(); ++cur) {

        char *cstr = new char[(cur->first).length() + 1];
        strcpy(cstr, (cur->first).c_str());

        printf("%*s[%s] [%s: %d] [scope: %d] [addr: %p] [table addr: %p]\n",
                3 * 0, "",
                cstr,
                "TType", cur->second->getTType()->getTTypeID(),
                cur->second->getScope(),
                cur->second,
                cur->second->getGamma());
        delete [] cstr;
    }
}

#endif
