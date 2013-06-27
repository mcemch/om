/* 
 * File:   list.h
 * Author: memch
 *
 * Created on January 10, 2011, 2:07 PM
 */

#ifndef LIST_H
#define	LIST_H

#include "om.h"

// list is a place holder for a set of static methods which
// are used by the hash class (symbol table).  this class
// is not meant for generic use.

class List {
public:
    List();
    List(const List& orig);
    virtual ~List();
    // these are linked list operations (stack operations)
    // for use with the hash class.
    static void push(Type **, Type *);
    static Type *pop(Type **);
    static Type *find(Type **, char*);
    static void remove(Type **, Type *);
private:

};

#endif	/* LIST_H */

