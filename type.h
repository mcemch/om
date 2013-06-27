/* 
 * File:   type.h
 * Author: memch
 *
 * Created on January 8, 2011, 1:45 AM
 */

#ifndef TYPE_H
#define	TYPE_H

#include "om.h"

// possible symbol types
enum data_types {
    type_INT, type_FLO, type_STR, type_METHOD
};

// type base class
class Type {
public:
    virtual char *valueAsString() = 0;
    char *getId(void);
    enum data_types type;
    friend class Hash;
    friend class List;
    friend class AST;
protected:
    char *id;
    Type *next;
};

// integer data type
class Int : public Type {
  public:
    // this is backwards: int value and string identifier
    // TODO: reverse these params
    Int(int, char *);
    char *valueAsString(void);
    int value;
  private:
      ;
};

// floating point data type
class Flo : public Type {
public:
    Flo(double, char *);
    char *valueAsString(void);
    double value;
private:
    ;
};


// string data type
class Str : public Type {
public:
    // this is (value,id)
    Str(char*, char*);
    char *valueAsString(void);
    char *value;
private:
    ;
};

// method symbol
class Method : public Type {
public:
    Method(char*, ParamList *);
    char *valueAsString(void);
    Node *start;
    ParamList *param;
    int param_length;
private:
    ;
};

#endif	/* TYPE_H */

