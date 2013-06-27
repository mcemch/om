/* 
 * File:   node.h
 * Author: memch
 *
 * Created on January 9, 2011, 4:44 PM
 */

#ifndef NODE_H
#define	NODE_H

#include "om.h"
#include "type.h"

// this is the type of node in the tree.  all possible
// node types are listed here.  the node type defines
// the action that will occur during evaluation of the tree.
enum node_type {
    node_STMT,	/* statement list */
    node_EXPR,  /* expression list */
    node_FC,	/* flow control list */
    node_LOOP,  /* loop statement */
    node_ARG,   /* argument node */
    node_BRK,   /* break statement */
    node_CON,	/* continue statement */
    node_PRT,	/* print statement */
    node_RET,   /* return statement */
};

// this is the type of operation performed on an expression node.
// these op types are only used in expression evaluation
enum op_type {
    op_ADD,    /* add */
    op_SUB,    /* sub */
    op_MUL,    /* mul */
    op_DIV,    /* div */
    op_MOD,    /* mod */
    op_AND,    /* && */
    op_LOR,    /* || */
    op_EQ,     /* == */
    op_NE,     /* != */
    op_GE,     /* >= */
    op_GT,     /* > */
    op_LT,     /* < */
    op_LE,     /* <= */
    op_CAT,    /* . */
    op_NOT,    /* ! */
    op_SET,    /* store */
    op_GET,    /* get value */
    op_CONST,  /* get constant */
    op_CALL,   /* function call */
    op_NULL,   /* no expression node*/
};

// union for values
union node_values {
    int v_int;
    double v_flo;
    char *v_str;
};


// the node class is used by the AST class.  the way the AST
// is formed bottom up by generating the parse tree in bison and
// then setting the root node to the method's symbol.  then
// at run time the ast is created by passing the method symbol
// to the AST constructor.  
class Node {
public:
    Node();
    Node(const Node& orig);
    Node(enum node_type);
    Node(enum node_type, enum op_type);
    Node(enum node_type, Node*, Node*);
    Node(enum node_type, enum op_type, Node*, Node*);
    virtual ~Node();
    char* valueAsString(void);
    // a node is an element of a binary parser tree so each
    // node must have a right and left child pointer
    Node *left;
    Node *right;
    // symbol's id if op_GET, op_SET, or op_CALL
    char *id;
    // node's storage
    // TOCOS: change to have temporary storage available in the ast?
    // instead of each node?
    // or should we have a symbol pointer here?
    // ***
    union node_values value;
    // storage data type
    enum data_types value_type;
    // node's type as defined above
    enum node_type type;
    // expression node's operation as defined above
    enum op_type op;
private:
    ;
};


#endif	/* NODE_H */

