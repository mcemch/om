/* 
 * File:   ast.cpp
 * Author: memch
 * 
 * Created on January 8, 2011, 5:25 AM
 */

#include "ast.h"
#include "node.h"
#include "type.h"
#include "hash.h"

// include the astprt functions when debugging
#ifdef T_DEBUG_FLAG
#include "astprt.cpp"
#endif

// default ast constructor.  we require the method's symbol pointer
// to set the root node and to obtain the method's identifier.  a
// new symbol table is created upon instance construction.
AST::AST(Method *method) {
    this->method = method;
    this->root = method->start;
    this->arg_list = new ArgList();
    this->symtab = new SymTab();
    this->ast_break = 0;
    this->ast_continue = 0;
    this->ast_return = 0;
    this->return_value.v_int = 0;
    this->has_return_value = 0;
    this->return_type = type_INT;
}

// wrapper function to pass the AST root node to the evaluation
// function
void AST::astEvaluate(void) {
    this->astEvaluate(root);
}

// this is the main evaluation routine. call this with the root node
// of the AST or use the wrapper function.
void AST::astEvaluate(Node *node) {
    // note that we want as few operations in this function as possible. if
    // we're looping 20 million times, a few unecessary operations can add
    // up quickly.  we also want as few function calls as possible.
    // expression evaluation will be inlined.
    if (node == NULL) return;

    // if we're a constant, return immediately
    if (node->op == op_CONST) return;

    // if the return flag is set we have to return immediately
    if (this->ast_return)  return;

    // if the continue or break flags are set we have to return
    // immediately also
    if (this->ast_break || this->ast_continue) return;

    // TODO:

    // convert tree traversal to iterative


    // this is a post order traversal but we're going to check the
    // flow control and loop nodes in the middle.
    (node->left)  ? astEvaluate(node->left) : (void)NULL;

    // a few nodes are special and need to be evaluated in order.
    // these are the nodes where the execution of the right child
    // depends on the value of a conditional this is.
    if(node->type == node_FC) {
	// here our left child is our conditional and the right
	// child is our conditional statements.  we return if
	// conditional is false so the right child subtree will not
	// get executed.
        if(!(node->left->value.v_int)) {
            return;
        }
    }

    if(node->type == node_LOOP) {
	// here we call astEvaluateLoop.  loop control has already been
	// evaluated so we'll check the value of our left->right grandchild
	// then call the function if true.  our left child should always
	// exist if the code parsed correctly.  we'll return if the conditional
	// is false
        if(node->left->right != NULL) {
            // note that if left->right == NULL we loop forever (for :)
            if(!node->left->right->value.v_int) {
            // if this is false then return
                return;
            }
        }
        this->astEvaluateLoop(node);
    }
    // everyone else needs this expression evaluated, even node_FC if
    // conditional was true
    else {
        (node->right) ? astEvaluate(node->right): (void)NULL;
    }

    // now everything required has been evaluated so we can use the values
    // for the appropriate statement
    switch(node->type) {
        case node_STMT:
            // statement list is a place holder for the sub trees.  they've been
            // evaluated so we don't need to do anything further.
            break;
        case node_EXPR:
            // this just switches on op->type and sets the appropriate value
            // in value.v_int
            astEvaluateExpr(node);
            break;

        case node_RET:
            // set some kind of rv here, reference implementation rv's are
            // totally broken.  if node's right child isn't null then there
            // was an expression evaluation.  set the rv to that
            if (node->right != NULL) {
                if (node->right->value_type == type_INT) {
                    this->return_value.v_int = node->right->value.v_int;
                    this->return_type = type_INT;
                    this->has_return_value = 1;
                } else if (node->right->value_type == type_FLO) {
                    this->return_value.v_flo = node->right->value.v_flo;
                    this->return_type = type_FLO;
                    this->has_return_value = 1;
                } else if (node->right->value_type == type_STR) {
                    this->return_value.v_str = node->right->value.v_str;
                    this->return_type = type_STR;
                    this->has_return_value = 1;
                }

                else {
                    errorExit("Unknown node value_type: %d", node->value_type);
                }
            }
            else {
                // TODO: remove me and add check for has_return_value in op_call
                this->has_return_value = 1;
                this->return_value.v_int = 0;
                this->return_type = type_INT;
            }
            this->ast_return = 1; return;
            break;

        case node_PRT:
            // this just prints out the value of our expression
            // remember node->right has already been evaluated at this point
            if(node->right != NULL) {
            // TOCOS: if we have value as a symbol pointer we can just
            // call sym->valueAsString() here and have one printf("%s")
                if (node->right->value_type == type_FLO) {
                    printf("%f\n", node->right->value.v_flo);
                } else if(node->right->value_type == type_STR) {
                    printf("%s\n", node->right->value.v_str);
                } else if(node->right->value_type == type_INT) {
                    printf("%d\n", node->right->value.v_int);
                } else {
                    errorExit("Unknown node value_type: %d", node->right->value_type);
                }
            } else {
                // empty prt statement
                printf("\n");
            }
            break;

        case node_BRK:
            // set a global variable for use in astEvaluateLoop
            // we will check this and set it to zero then break
            this->ast_break = 1; return;
            break;

        case node_CON:
            // set a global variable for use in astEvaluateLoop
            // we will check this, set it to zero, then continue
            this->ast_continue = 1; return;
            break;

        case node_ARG:
            // assign the expression result to the node and add to
            // the tree's arg_list
            node->value_type = node->right->value_type;
            if (node->value_type == type_INT) {
                node->value.v_int = node->right->value.v_int;
            } else if(node->value_type == type_FLO) {
                node->value.v_flo = node->right->value.v_flo;
            } else if(node->value_type == type_STR) {
                node->value.v_str = node->right->value.v_str;
            } else {
                errorExit("Unknown node value_type: %d", node->value_type );;
            }
            this->arg_list->push_front(node);
            break;

        // if we returned here conditional was false
        // we don't have to do anything else with this node
        case node_FC: // if
            // if we make it here then conditional was false
            break;

        // loop node has already done its work
        case node_LOOP: // for
            break;

        // this should never happen
        default:
            errorExit("Node type not implemented: %d", node->type);
            break;
        }
    }


// this is the expression evalutaion function.  this function should only be
// called if node_type == node_EXPR
inline void AST::astEvaluateExpr(Node *node) {
    if (node == NULL) return;

    // use this variable to assign to node storage
    Type *sym;

    // the operations are pretty much self-explanitory, what we do here
    // is perform the binop on the left and right child and store the
    // result in the node's union field
    switch (node->op) {

    // arithmetic operations
	case op_ADD:
	    // does this make sense?  we check if they're both floats, if so then assign
	    // to that storage, then check if any of the children values are floats and
	    // assign to float storage, finally we assign to integer storage if both
	    // child nodes are of type int.  it seems like a lot of code but if we move
	    // this into functions the performance penalty is unacceptable.
        if(node->left->value_type == type_STR || node->right->value_type == type_STR) {
            errorExit("Operation not supported on string type: %d", node->op);
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo + node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo + node->right->value.v_int;
            node->value_type = type_FLO; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_int + node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else {
            node->value.v_int = node->left->value.v_int + node->right->value.v_int;
            node->value_type = type_INT; }
	    break;
	case op_SUB:
        if(node->left->value_type == type_STR || node->right->value_type == type_STR) {
            errorExit("Operation not supported on string type: %d", node->op);
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo - node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo - node->right->value.v_int;
            node->value_type = type_FLO; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_int - node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else {
            node->value.v_int = node->left->value.v_int - node->right->value.v_int;
            node->value_type = type_INT; }
	    break;
	case op_MUL:
        if(node->left->value_type == type_STR || node->right->value_type == type_STR) {
            errorExit("Operation not supported on string type: %d", node->op);
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo * node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo * node->right->value.v_int;
            node->value_type = type_FLO; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_int * node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else {
            node->value.v_int = node->left->value.v_int * node->right->value.v_int;
            node->value_type = type_INT; }
	    break;
	case op_DIV:
        if(node->left->value_type == type_STR || node->right->value_type == type_STR) {
            errorExit("Operation not supported on string type: %d", node->op);
        }
        else if ((node->right->value_type == type_INT) && (node->right->value.v_int == 0)) {
            errorExit("Divide by zero error: %s(%d)", node->right->id, node->right->value.v_int);
	    }
        else if ((node->right->value_type == type_FLO) && (node->right->value.v_flo == (double)0.0)) {
            errorExit("Divide by zero error: %s(%f)", node->right->id, node->right->value.v_flo);
	    }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo / node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_flo / node->right->value.v_int;
            node->value_type = type_FLO; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_flo = node->left->value.v_int / node->right->value.v_flo;
            node->value_type = type_FLO; }
	    else {
            node->value.v_int = node->left->value.v_int / node->right->value.v_int;
            node->value_type = type_INT; }
	    break;
	case op_MOD:
	    if (node->left->value_type != type_INT || node->right->value_type != type_INT) {
            errorExit("Modulus operator only supported on integer type: %d: %d",
                    node->left->value_type, node->right->value_type);
	    } else {
	        node->value.v_int = node->left->value.v_int % node->right->value.v_int;
	        node->value_type = type_INT;
        }
	    break;



	// comparison operations
  case op_AND:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), "0",
                strlen(node->left->valueAsString())) != 0 &&
                strncmp(node->left->valueAsString(), "0",
                strlen(node->right->valueAsString()) != 0)) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo && node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo && node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int && node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int && node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
    case op_LOR:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), "0",
                strlen(node->left->valueAsString())) != 0 ||
                strncmp(node->left->valueAsString(), "0",
                strlen(node->right->valueAsString()) != 0)) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo || node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo || node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int || node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int || node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_EQ:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), node->right->valueAsString(),
                strlen(node->left->valueAsString())) == 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo == node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo == node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int == node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int == node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_NE:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(),node->right->valueAsString(),
                strlen(node->left->valueAsString())) != 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo != node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo != node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int != node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int != node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_GE:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), node->right->valueAsString(),
                strlen(node->left->valueAsString())) >= 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo >= node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo >= node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int >= node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int >= node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_GT:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), node->right->valueAsString(),
                strlen(node->left->valueAsString())) > 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo > node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo > node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int > node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int > node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_LE:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), node->right->valueAsString(),
                strlen(node->left->valueAsString())) <= 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo <= node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo <= node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int <= node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int <= node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
	case op_LT:
        if (node->left->value_type == type_STR || node->right->value_type == type_STR) {
            if (strncmp(node->left->valueAsString(), node->right->valueAsString(),
                strlen(node->left->valueAsString())) < 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if (node->left->value_type == type_FLO && node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo < node->right->value.v_flo; }
	    else if(node->left->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_flo < node->right->value.v_int; }
	    else if(node->right->value_type == type_FLO) {
            node->value.v_int = node->left->value.v_int < node->right->value.v_flo; }
	    else {
            node->value.v_int = node->left->value.v_int < node->right->value.v_int; }
        node->value_type = type_INT;
	    break;
    // string concatination operation.  here we'll cast everything to a
    // string
    case op_CAT: {
        char *str_left = node->left->valueAsString();
        char *str_right = node->right->valueAsString();
        char *str_new = strCat(str_left, str_right);
        node->value_type = type_STR;
        node->value.v_str = str_new;
        break; }

    // unary operator for negation.  this only has the right child node.
	case op_NOT:
         if (node->right->value_type == type_STR) {
            if (strncmp(node->left->value.v_str, "0",
                strlen(node->left->value.v_str)) == 0) {
                node->value.v_int = 1;
            } else {
                node->value.v_int = 0;
            }
        }
        else if(node->right->value_type == type_FLO) {
            node->value.v_int = !node->right->value.v_flo ; }
	    else {
            node->value.v_int = !node->right->value.v_int; }
        node->value_type = type_INT;
	    break;



	case op_GET:
	    // here we check the symbol table for the node. if it doesn't
	    // exist, then we haven't 'set' it yet so we exit with an
	    // error.  if it is there, then we place it's value in the
	    // node's storage field 'union value'
	    if((sym = this->symtab->get((char*)node->id)) == NULL) {
		    errorExit("Unknown symbol: %s", node->id);
	    }
	    if (sym->type == type_INT) {
            Int *temp = (Int *)sym;
		    node->value.v_int = temp->value;
            node->value_type = type_INT;
	    } else if(sym->type == type_FLO) {
            Flo *temp = (Flo *)sym;
		    node->value.v_flo = temp->value;
            node->value_type = type_FLO;
        } else if (sym->type = type_STR) {
            Str *temp = (Str *)sym;
            node->value.v_str = temp->value;
            node->value_type = type_STR;
	    } else {
            errorExit("Unknown symbol type: %d", sym->type);
        }
	    break;


	case op_SET:
	    Type *type;
        // remember that we can use assignments in expression evaluation
        // like in C.  so the first thing we do is set our node's value
        // and value type
        node->value_type = node->right->value_type;
        if(node->value_type == type_INT) {
            node->value.v_int = node->right->value.v_int;
        } else if (node->value_type == type_FLO) {
            node->value.v_flo = node->right->value.v_flo;
        } else if (node->value_type == type_STR) {
            node->value.v_str = node->right->value.v_str;
        } else {
            errorExit("Unknown node value type: %d", node->value_type);
        }
	    // check the symbol table for the symbol, if it is already there
	    // then all we have to do is check its type and then
        // assign its value, otherwise we create
	    // a new symbol and store it.
	    if(!(type = this->symtab->get((char*)node->id))) {
            if(node->value_type == type_INT) {
                type = new Int(node->value.v_int, node->id);
            }
            else if(node->value_type == type_FLO) {
                type = new Flo(node->value.v_flo, node->id);
            }
            else if(node->value_type == type_STR) {
                type = new Str(node->value.v_str, node->id);
            }
            else {
                errorExit("Unknown symbol type: %d", type->type);
            }
		    this->symtab->put(type);
		    M_DEBUG("%p: %s: added symbol: %s", this->symtab,
			    this->method->getId(), node->id);
	    } else {
            // if the symbol already exists here then the type may have changed.
            // check each current type and either cast or create a new type as
            // appropriate.  int gets a new type if current value is type float
            // or string.  float gets a new type if current value is a string,
            // otherwise value is casted to a float.  if the type is a string,
            // the value gets casted to a string.
            switch(type->type) {
                case type_INT: {
                    switch(node->value_type) {
                        case type_INT: {
                            Int *temp = (Int*)type;
                            temp->value = node->value.v_int;
                            break; }
                        case type_FLO: {
                            Flo *temp = new Flo(node->value.v_flo, type->id);
                            this->symtab->remove(type->id);
                            this->symtab->put(temp);
                            break; }
                        case type_STR: {
                            Str *temp = new Str(node->value.v_str, type->id);
                            this->symtab->remove(type->id);
                            this->symtab->put(temp);
                            break; }
                        default: {
                            errorExit("Unknown node value_type: %d", node->value_type);
                            break; }
                    }
                    break; }
                case type_FLO: {
                    switch(node->value_type) {
                        case type_INT: {
                            Flo *temp = (Flo*)type;
                            temp->value = (double)node->value.v_int;
                            break; }
                        case type_FLO: {
                            Flo *temp = (Flo*)type;
                            temp->value = node->value.v_flo;
                            break; }
                        case type_STR: {
                            Str *temp = new Str(node->value.v_str, type->id);
                            this->symtab->remove(type->id);
                            this->symtab->put(temp);
                            break; }
                        default: {
                            errorExit("Unknown node value_type: %d", node->value_type);
                            break; }
                    }
                    break; }
                case type_STR: {
                    switch(node->value_type) {
                        case type_INT: {
                            Str *temp = (Str*)type;
                            temp->value = (char*)node->valueAsString();
                            break; }
                        case type_FLO: {
                            Str *temp = (Str*)type;
                            temp->value = (char*)node->valueAsString();
                            break; }
                        case type_STR: {
                            Str *temp = (Str*)type;
                            temp->value = (char*)node->value.v_str;
                            break; }
                        default: {
                            errorExit("Unknown node value_type: %d", node->value_type);
                            break; }
                    }
                    break; }
                default: {
                    errorExit("Unknown symbol type: %d", type->type);
                    break; }
            }
	    }
	    break;


	case op_CALL: {
        // all methods are currently stored in the global symbol table
        // if this method doesn't exist we have to bail
        // the arg_list should already be created
        Method *method = (Method *)G_SYMTAB->get(node->id);
        if (method == NULL) {
             errorExit("Call to undefined method: %s", node->id);
         }

        // TODO:
        // not sure how to verify function parameters of the form:
        // .f1(x, y, .f2(x, y), z)
        // ADD A CALL STACK FOR PARAMETERS
        // new parms_list
        // parms_list.push(x)
        // parms_list.push(y)
        // call_stack.push(parms_list)
        // new parms_list
        // parms_list.push(x)
        // parms_list.push(y)
        // result = call .f2()
        // parms_list.empty()
        // parms_list = call_stack.pop()
        // parms_list.push(z)
        // result = call .f1()
        // return result


        // TODO:
        // get rid of the AST GLOBAL variables and add them to the
        // AST instance itself

        // each method call is a new AST complete with it's own symbol
        // table, control variables, and return value.  all we require
        // to create a new AST instance is the method's symbol
        AST *ast = new AST(method);
        ast->astSetParameters(this->arg_list);
        ast->astEvaluate();

        // grab return value
        // where this->return is set by the ret statement
        if (ast->has_return_value) {
            node->value_type = ast->return_type;
            switch(node->value_type) {
                case type_INT: node->value.v_int = ast->return_value.v_int; break;
                case type_FLO: node->value.v_flo = ast->return_value.v_flo; break;
                case type_STR: node->value.v_str = ast->return_value.v_str; break;
                default:
                    errorExit("Unknown return type: %d", ast->return_type);
                    break;
            }
        }

#ifdef M_DEBUG_FLAG
        // dump the final symbol table
        ast->symtab->dump();
#endif
        // if function is void might as well set it to zero
        // might change this later

        // now we need to clean up as best we can
        // see the function destructor
        delete ast;
        this->arg_list->empty();
        break; }

    // add more operations here...

    case op_NULL:
        // null operations are expression node place holders.  these can probably
        // be removed at some point.
        break;


	default:
        // remember that we cannot evaluate op_CONST
	    errorExit("Operation not implemented: %d", node->op);
	    break;
    }

};

// this is our looping function, we have to check for the global
// break and continue flags and for return statements
void AST::astEvaluateLoop(Node *node) {
    if (node == NULL) {
        return;
    }
    // ok so our node->left is our loop control node, we can assume
    // at this point initial conditions been set and our conditional
    // is true... the subtree looks like this
    // node_LOOP
    //  ->left node_LCTL                   |   ->right  node_STMT
    //  ->left initial ->right branch_cond |   ->left ... -> right ...
    // so here we need to re-evaluate branch cond at the end
    // and execute the statement list on the right
    int loop_cond = 1;
    do {
        this->astEvaluate(node->right);
	// if we return here with a break or continue flag set then
	// we perform the commands and reset the flags
        if (this->ast_break) {
            this->ast_break = 0;
            break; // this is the break
        }
        if (this->ast_continue) {
            this->ast_continue = 0;
            continue; // this should work
        }
        // we should always have a left child LCTL node, but if our
        // branching conditional is null then we loop forever
        if(node->left->right != NULL)  {
            astEvaluate(node->left->right);
            // note this will never get set if no conditional is
            // specified...
            loop_cond = node->left->right->value.v_int;
        }
    } while (loop_cond);
}



// this function sets the parameters defined in the symbol's param to those
// present int the instance's arg_list.  both list are currently stored in a
// list stacked in reverse order
void AST::astSetParameters(ArgList *argv) {
    // parameter list is string id's and argument list is node *'s
    // so we need to make sure they match up
    ParamList *p_list = this->method->param;
    ArgList *a_list = argv;
    Node *arg;

    int a_size = a_list->size();
    int p_size = this->method->param_length;

    // check for too many, too few, arguments
    if (p_size > a_size) {
        errorExit("Not enough arguments to method: %s", this->method->getId());
    }
    else if (p_size < a_size) {
        errorExit("Too many arguments to method: %s", this->method->getId());
    }
    else {
        // if we have no parameters then exit
        if (p_size == 0)  return;

        // iterate through parameter list creating a new type based
        // on arg->value and current parameter id.  add the type to the
        // symbol table and pop the type from the arg_list
        ParamList::iterator parm;
        for (parm=p_list->begin(); parm != p_list->end(); ++parm) {
            M_DEBUG("%p: %s: adding symbol: %s", symtab, method->getId(), *parm);
            arg = a_list->front();
            switch(arg->value_type) {
                case type_INT: {
                    Int *sym = new Int(arg->value.v_int, *parm);
                    symtab->put(sym);
                    break; }
                case type_FLO: {
                    Flo *sym = new Flo(arg->value.v_flo, *parm);
                    symtab->put(sym);
                    break; }
                case type_STR: {
                    Str *sym = new Str(arg->value.v_str, *parm);
                    symtab->put(sym);
                    break; }
                default: {
                    errorExit("Unknown node value_type: %d", arg->value_type);
                    break; }
            }
            a_list->pop_front();
        }
#ifdef M_DEBUG_FLAG
        symtab->dump();
#endif

    }
}



