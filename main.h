/* 
 * File:   main.h
 * Author: memch
 *
 * Created on January 8, 2011, 1:22 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "om.h"
#include "type.h"
#include "hash.h"
#include "list.h"
#include "node.h"
#include "ast.h"

// function prototypes for lexer and parser
int yylex(void);
void yyerror(const char *,...);

// externally defined global variables
extern FILE *yyin;
extern int errno;

// put global variables here
int linenum = 1;
SymTab     *G_SYMTAB;
ParamList  *G_PARAM_LIST;


#endif	/* MAIN_H */

