/* 
 * File:   om.h
 * Author: memch
 *
 * Created on January 8, 2011, 1:18 AM
 */

#ifndef OM_H
#define	OM_H

// every header file at a minimum includes om.h
// and every source file at a minumum includes its
// corresponding header file, unless the source file
// is itself included somewhere else.

// place C/C++ library includes in here
#include "common.h"

// declare all classes here.  some of the
// classes are used in others, so it is good
// to keep track of all we're defining
class Type;
class Int;
class Method;
class Node;
class AST;
class Hash;
class List;

// put typedefs here if possible
typedef std::string String;
typedef std::list<char *> ParamList;
typedef std::list<Node *> ArgList;
typedef class Hash SymTab;

// debugging options
// #define   DEBUG_MODE 1

#ifdef DEBUG_MODE
#define   L_DEBUG_FLAG 1 /* lexer debugging */
#define   P_DEBUG_FLAG 1 /* parser debugging */
#define   M_DEBUG_FLAG 1 /* symbol table debugging */
#define   T_DEBUG_FLAG 1 /* ast debugging */
#endif

#ifdef    L_DEBUG_FLAG
  #define L_DEBUG(...) debug(__VA_ARGS__)
#else
  #define L_DEBUG(...) ;
#endif

#ifdef    P_DEBUG_FLAG
  #define P_DEBUG(...) debug(__VA_ARGS__)
#else
  #define P_DEBUG(...) ;
#endif

#ifdef    T_DEBUG_FLAG
  #define T_DEBUG(...) debug(__VA_ARGS__)
#else
  #define T_DEBUG(...) ;
#endif

#ifdef    M_DEBUG_FLAG
  #define M_DEBUG(...) debug(__VA_ARGS__)
#else
  #define M_DEBUG(...) ;
#endif


#endif	/* OM_H */
