#include <string>

#include "type.h"

// return the symbol's identifier
char *Type::getId(void) {
    return id;
}


// pass the value and the symbol's id
Int::Int(int i, char *str) {
    id = str;
    next = NULL;
    type = type_INT;
    value = i;
}


// returns the integer value as string
char *Int::valueAsString(void) {
    std::stringstream ss;
    ss << value;
    return (char *)ss.str().c_str();
}


// double data type
Flo::Flo(double d, char *str) {
    id = str;
    next = NULL;
    type = type_FLO;
    value = d;
}

// returns the double value as a string
char *Flo::valueAsString(void) {
    std::stringstream ss;
    ss << value;
    return (char *)ss.str().c_str();
}


// string data type
Str::Str(char *v, char *str) {
    id = str;
    next = NULL;
    type = type_STR;
    value = v;
}

// returns char* value
char *Str::valueAsString(void) {
    return value;
}


// method symbol constructor for type_METHOD
Method::Method(char *str, ParamList *list) {
    id = str;
    next = NULL;
    type = type_METHOD;
    start = NULL;
    // if the list is null we have no parameters, so we
    // set length to 0
    param = list;
    if (list == NULL) {
	    param_length = 0;
    } else {
	    param_length = list->size();
    }
}


// returns the parameter list as a string
char *Method::valueAsString(void) {
    std::stringstream ss;
    char *s;
    ss << "params: ";
    if(param != NULL)  {
        ParamList::iterator item;
        for(item=param->begin(); item != param->end(); ++item )
            ss << *item << " ";
    }

    s = strMalloc((char *)ss.str().c_str(), ss.str().length());
    return s;
}


