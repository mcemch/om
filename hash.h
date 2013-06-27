/* 
 * File:   hash.h
 * Author: memch
 *
 * Created on January 10, 2011, 1:47 PM
 */

#ifndef HASH_H
#define	HASH_H

#include "om.h"

// this is a C implementation of a hash table using
// linked list implementation in list.{h,cpp}.  the following
// operations are supported:
// this->put(symbol)   	# stores a type in the symbol table
// symbol = this->get(identifier) # retrieves a symbol
// this->dump();	# print the symbol table

// we used to use std::map<std::string,Type *>
// but it was way slow.  this is heavily accessed during
// evaluation and needs to be as resource efficient as possible.

class Hash {
public:
    Hash();
    Hash(const Hash& orig);
    virtual ~Hash();
    void put(Type*);
    Type *get(char*);
    void remove(char*);
    void dump();
private:
    // size of the symbol table area
    int M_SYMSIZE;
    // symbol table pointer
    Type **symtab;
    // this is djb's hashing function
    int h(char *);
    // this allocates storage for the symtab and is called
    // by the constructor
    Type **create();
};

#endif	/* HASH_H */

