/* 
 * File:   common.h
 * Author: memch
 *
 * Created on January 8, 2011, 1:35 AM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <iostream>
#include <sstream>
#include <list>

#include <errno.h>
#include <string.h>
#include <strings.h>

// put macro's here
#define print 	std::cout << 
#define endl    std::endl

// used in errorExit and debug functions
extern int linenum;

void debug(const char *, ...);
void errorExit(const char*, ...);
void *commonMalloc(unsigned);
char *strMalloc(const char *, size_t);
char *strMallocSubStr(const char *, size_t, int, int);
char *strCat(char *, char*);

#endif	/* COMMON_H */

