/*
 * File:   astprt.cpp
 * Author: memch
 *
 * Created on January 8, 2011, 5:25 AM
 */

#include "ast.h"
#include "node.h"

// this is a separate file for printing the tree.  this is only used
// when debugging is enabled.  this is controlled by the
// T_DEBUG_FLAG macro as set in "om.h".

// wrapper function for printing th whole tree starting at
// the root node.
void AST::astPrint() {
    this->astPrint(root);
}


// this is an in-order traversal of the tree.  this is meant for
// debugging purposes only
void AST::astPrint(Node *node) {
    if(node == NULL) {
        return;
    }

    // print the left subtree
    this->astPrint(node->left);

    print "Node: ";
    printf("%p: ", node);

    // print node type
    switch(node->type) {
	case node_STMT: print "stmt_list :" << endl; break;
	case node_FC:   print "stmt_if   :" << endl; break;
	case node_LOOP: print "stmt_for  :" << endl; break;
	case node_ARG:  print "node_arg  :" << endl; break;
	case node_BRK:  print "stmt_brk  :" << endl; break;
	case node_CON:  print "stmt_con  :" << endl; break;
	case node_PRT:  print "stmt_prt  :" << endl; break;
	case node_RET:  print "stmt_ret  :" << endl; break;
	case node_EXPR:
	    print "expression: ";
            // if we're an expression, we have an operation
	    switch (node->op) {
		case op_ADD:  print "   op: +" << endl; break;
		case op_SUB:  print "   op: -" << endl; break;
		case op_MUL:  print "   op: *" << endl; break;
		case op_DIV:  print "   op: /" << endl; break;
		case op_MOD:  print "   op: %" << endl; break;
		case op_AND:  print "   op: &&" << endl; break;
		case op_LOR:  print "   op: ||" << endl; break;
		case op_EQ:   print "   op: ==" << endl; break;
		case op_NE:   print "   op: !=" << endl; break;
		case op_GE:   print "   op: >=" << endl; break;
		case op_GT:   print "   op: >" << endl; break;
		case op_LE:   print "   op: <=" << endl; break;
		case op_LT:   print "   op: <" << endl; break;
		case op_NOT:  print "   op: !" << endl; break;
		case op_CAT:  print "   op: |" << endl; break;
		case op_SET:  printf("  set: %s = \n", node->id); break;
		case op_CONST:printf("const: %s\n", node->id); break;
		case op_GET:  printf("  get: %s\n", node->id); break;
		case op_CALL: printf(" call: %s\n", node->id); break;
		case op_NULL: print endl; break;
		default:
		    errorExit("Operation not implemented: %d", node->op);
		    break;
	    }
	    break;
	default:
	    errorExit("Node type not implemented: %d", node->type);
	    break;
    }

    // print the right subtree
    this->astPrint(node->right);
}


void AST::astPrintIter() {


}

