/* 
 * File:   ast.h
 * Author: memch
 *
 * Created on January 8, 2011, 5:25 AM
 */

#ifndef AST_H
#define	AST_H

#include "om.h"
#include "type.h"
#include "node.h"

// the global symbol table is created in main.cpp.  this is where
// constant and method identifiers are stored.
extern SymTab* G_SYMTAB;

// the AST class provides a symbol table, argument list, root node
// pointer, control flags, and methods for evaluating the tree nodes.
// if T_DEBUG_FLAG is enabled, the method also provides methods for
// printing the tree.
class AST {
public:
    AST(Method *);
    void astEvaluate();
    void astEvaluate(Node*);
#ifdef T_DEBUG_FLAG
    void astPrint();
    void astPrint(Node *);
    void astPrintIter();
#endif
    friend int main(int, char**);
private:
    ArgList *arg_list;
    SymTab *symtab;
    Node *root;
    Method *method;
    union node_values return_value;
    int has_return_value;
    enum data_types return_type;
    int ast_break;
    int ast_continue;
    int ast_return;
    void astEvaluateExpr(Node *);
    void astEvaluateLoop(Node *);
    void astSetParameters(ArgList *);
};

#endif	/* AST_H */

