/*
 * File:   node.cpp
 * Author: memch
 *
 * Created on January 8, 2011, 5:25 AM
 */

#include "node.h"

// basic constructor is a statement node
Node::Node() {
    id = NULL;
    type = node_STMT;
    op = op_NULL;
    left = NULL;
    right = NULL;
}

// fill me in for copying. currently we don't need it.
Node::Node(const Node& orig) {
}

// creates a basic node of type with operation op_NULL
Node::Node(enum node_type type) {
    id = NULL;
    left = NULL;
    right = NULL;
    this->type = type;
    this->op = op_NULL;
}

// creates a node of type and operation op with left and right
// set to null.  these are public attributes so you can set
// them after the node is created
Node::Node(enum node_type type, enum op_type op) {
    id = NULL;
    left = NULL;
    right = NULL;
    this->type = type;
    this->op = op;
}

// creates a node of type type and op_NULL given left and right
// specified as arguments.  use this constructor for nodes
// other than expression nodes, e.g. node_STMT.
Node::Node(enum node_type type, Node *left, Node *right) {
    id = NULL;
    this->type = type;
    this->op = op_NULL;
    this->left = left;
    this->right = right;
}

// creates a node of type and operation op given left and right
// specified as arguments.  use this constructor for nodes of
// type node_EXPR
Node::Node(enum node_type type, enum op_type op, Node *left, Node *right) {
    id = NULL;
    this->type = type;
    this->op = op;
    this->left = left;
    this->right = right;
    this->value_type = type_INT;
}

// function destruction
Node::~Node() {
}


// returns the value of the node's storage as a string for use
// with string comparison.  1 == "1" is a valid statement for now
char *Node::valueAsString() {
    std::stringstream ss;
    char *str;
    switch (this->value_type) {
	case type_INT:
	    ss << this->value.v_int;
	    str = strMalloc(ss.str().c_str(), ss.str().length());
	    break;
	case type_FLO:
	    ss << this->value.v_flo;
	    str = strMalloc(ss.str().c_str(), ss.str().length());
	    break;
	case type_STR:
	    str = this->value.v_str;
	    break;
	default:
	    errorExit("Unknown node value_type: %d", this->value_type);
	    break;
    }
    return str;
}



