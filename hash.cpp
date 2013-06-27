/* 
 * File:   hash.cpp
 * Author: memch
 * 
 * Created on January 10, 2011, 1:47 PM
 */

#include "hash.h"
#include "list.h"
#include "type.h"

#ifdef T_DEBUG_FLAG
#include "ast.h"
#endif


// here symbol table width (number of buckets) is set.
// TODO: make M_SYMSIZE specifiable as an option for
// experimentation purposes.

Hash::Hash() {
    M_SYMSIZE = 3089;
    symtab = this->create();
}

Hash::Hash(const Hash& orig) {
}

Hash::~Hash() {
}

// braces are removed from these functions as a reminder that
// they're well tested and should not be modified unless absolutely
// necessary.

int Hash::h(char *string) {
    // this is djb's hashing function.
    unsigned int hash = 5381;
    unsigned int i    = 0;
    unsigned int len  = strlen(string);
    for(i = 0; i < len; string++, i++) 
       hash = ((hash << 5) + hash) + (*string);
    return (int)(hash & 0x7FFFFFFF) % M_SYMSIZE;
}


void Hash::put(Type *sp) {
    // first thing we do is get a hash of the character string
    // identifier modded with M_SYMSIZE to determine our hash
    // bucket.  then we look for the symbol in the bucket's
    // chain (list) which is ordered as a lifo stack.  that doesn't
    // really matter since we never remove elements from the
    // symbol table until it is destroyed.
    Type *np, *tp;
    int b = h(sp->id);
    if((np = List::find(&symtab[b],sp->id)) == NULL)
        List::push(&symtab[b], sp);
    else { tp = np; np = sp; free(tp); }
}


Type *Hash::get(char *id) {
    // return the symbol identified by id.  we obtain
    // the bucket by hashing the identifier and obtain the
    // head of the chain (list).  then we pass the identifier
    // string to list::find to walk the list.
    return List::find(&symtab[h(id)], id);
}


void Hash::remove(char *id) {
    // we need the id because we need to get the bucket again.
    // then we call list remove on the element
    Type *tp = this->get(id);
    List::remove(&symtab[h(id)], tp);
}


Type **Hash::create() {
    // create storage area for the symbol table.  we allocate
    // the size of a symbol pointer times the size of the
    // hash table (number of buckets) and then initialize
    // each memory element to null;
    Type **sp; int i;
    sp = (Type **) commonMalloc(sizeof(struct Type*) * M_SYMSIZE);
    for(i = 0; i < M_SYMSIZE; i++) sp[i] = NULL;
    return sp;
}


#ifdef M_DEBUG_FLAG
void Hash::dump() {
    // here we print out the symbol table values if the debugging flag is set
    int i;
    Type *sp;
#ifdef T_DEBUG_FLAG
    AST *ast;
#endif

    for(i = 0; i < M_SYMSIZE; i++) {
	// we have to step through each bucket in the hash table then
	// walk the list if it is present.
        if((sp = symtab[i]) != NULL) {
            do {
                switch(sp->type) {
                    case type_INT:
                        printf("%p: Symbol='%s' Value='%s' Bucket='%04d'\n", \
                            symtab, sp->id, sp->valueAsString(), i);
                        break;

                    case type_FLO:
                        printf("%p: Symbol='%s' Value='%s' Bucket='%04d'\n", \
                            symtab, sp->id, sp->valueAsString(), i);
                        break;

                    case type_STR:
                        printf("%p: Symbol'%s' Value='%s' Bucket='%04d'\n", \
                            symtab, sp->id, sp->valueAsString(), i);
                        break;

                    case type_METHOD:
                        printf("%p: Symbol='%s' Value='%s' Bucket='%04d'\n", \
                            symtab, sp->id, sp->valueAsString(), i);
#ifdef T_DEBUG_FLAG
                        ast = new AST((Method *)sp);
                        ast->astPrint();
                        delete ast;
#endif
                        break;

                    default:
                        errorExit("Unknown symbol type in symtab: %d", sp->type);
                        break;

                }
            } while((sp = sp->next) != NULL);
        }
    }
}
#endif





