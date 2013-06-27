/* 
 * File:   list.cpp
 * Author: memch
 * 
 * Created on January 10, 2011, 2:07 PM
 */

#include "list.h"
#include "type.h"
#include "hash.h"

List::List() { }

List::List(const List& orig) { }

List::~List() { }

// list methods are static so call as follows:
// List::push(symtab, symbol)
// symbol = List::pop(symbol)
// symbol = List::find(symbol)
// List::remove(symbol)


// note: this->pop() isn't used in the symbol table (hash).
// it is maintained here in case we need a stack of symbols
// we can search through by identifier


void List::push(Type **head, Type *sym) {
    // if list is empty set symbol's next pointer
    // to null.  otherwise, set symbols next pointer
    // to the current head.  then set the current
    // head to the symbol itself.  this pushes a value
    // onto the list.
    if(*head == NULL) { sym->next = NULL; }
    else { sym->next = *head; }
    *head = sym;
}


Type *List::pop(Type **head) {
    // if the list is empty, do nothing.  otherwise, set the
    // head of the list to temp (sp), set current head to
    // temp's next node, then return temp.  it is no longer in
    // the list chain so it is 'popped'.
    Type *sp; if (*head == NULL) { return NULL; }
    else { sp = *head; *head = sp->next; }
    return sp;
}


Type *List::find(Type **head, char *id) {
    // removing whitespace makes your code run faster.  lol, so
    // we return if the list is empty.  if not, then set temp (sp)
    // to head and loop.  if given 'id' == temp->id break, otherwise
    // set temp to temp->next.  this effectively walks the chain
    // until the next node is null.
    Type *sp; if(*head == NULL) return NULL;
    sp = *head; do {
        if(strcmp(sp->id, id) == 0) break;
        sp = sp->next;
    } while(sp != NULL);
    return sp;
}


void List::remove(Type **head, Type *sp) {
    // here we need a previous pointer as well as a temporary pointer.
    // if the head of the list is what we're removing, we set head to
    // sp's next node, then delete it.  otherwise, we walk the list
    // until we find our node and connect the previous and next elements
    Type *tp; Type *prev = *head;
    if(*head == sp) { *head = prev->next; delete sp; return; }
    for(tp = prev->next; tp != NULL; tp = tp->next) {
        if(tp == sp) { prev->next = tp->next; delete sp; return; }
        prev = tp; }
}
