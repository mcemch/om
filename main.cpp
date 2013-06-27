// main.cpp

using namespace std;

// start
int main(int argc, char *argv[]) {
    FILE *fin = NULL;
    int return_value = 0;

    /* TODO: use getopts */

    // check for too many arguments to program
    if(argc > 2) {
        printf("usage: omc [filename]\n");
        exit(1);
    }

    // if a filename is specified, open it and set
    // yyin to the file pointer
    if(argc == 2) {
        fin = fopen(argv[1], "rt");

        if(fin == NULL) {
	    errorExit("Couldn't open file: %s", argv[1]);
        } else {
            yyin = fin;
        }
    }

    // initalize global variables here
    G_SYMTAB = new SymTab();
    G_PARAM_LIST = NULL;

    // yacc it up
    if(yyparse()) {
	errorExit("Invalid syntax: %s", argv[1]);
    }

    // close the input file
    if(fin != NULL) {
	fclose(fin);
    }

    // now all functions should be stored in the global symbol table
    // and the AST for each method should be created.  each program
    // must have a main function, as in c.

#ifdef M_DEBUG_FLAG
    G_SYMTAB->dump();
#endif

    // main it up
    Method *method;
    char *main_id = strMalloc("main", 4);
    if((method = (Method *)G_SYMTAB->get(main_id)) == NULL) {
        errorExit("Method 'main' not defined.");
    }
    // this is essentially the same procedure as an op call.
    // we have no arguments, main is in the global symbol table
    // and the AST constructor will initialize everything for us
    AST *ast = new AST(method);

    // this essentially calls method->start
    ast->astEvaluate();
    if (ast->has_return_value) {
        if (ast->return_type == type_FLO) {
            return_value = (int)ast->return_value.v_flo;
        } else {
            return_value = ast->return_value.v_int;
        }
    }

#ifdef M_DEBUG_FLAG
    ast->symtab->dump();
#endif

    // see AST::~AST()
    delete ast;


    return return_value;
}


