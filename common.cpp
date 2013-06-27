#include "common.h"

void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stdout, "DEBUG: ");
    vfprintf(stdout, format, args);
    va_end(args);
    fprintf(stdout, "\n");
    fflush(stdout);
}

void errorExit(const char *format, ...) {
    va_list args;

    va_start(args, format);
    fprintf(stderr, "%3d: ERROR: ", linenum);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
    exit(1);
}


void *commonMalloc(unsigned s) {
    void *p;
    if((p = malloc(s)) == NULL)
        errorExit("Malloc: out of memory: %s\n", strerror(errno));
    return p;
}


char *strMalloc(const char *s, size_t len) {
    register char *c  = (char *) commonMalloc(len + 1);
    c[len] = '\0';
    return (char *)memcpy(c, s, len);
}



char *strMallocSubStr(const char *s, size_t len, int begin, int end) {
    int new_len = end - begin;
    if (new_len > len) {
	errorExit("SubString allocation size error: %d > %d", new_len, len);
    }
    register char *c = (char *) commonMalloc((new_len)+1);
    c[new_len] = '\0';
    for(int i=0; i<(new_len); i++) {
	c[i] = s[begin+i];
    }
    return c;
}


char *strCat(char *left, char *right) {
    int left_len = strlen(left);
    int right_len = strlen(right);
    int new_len = left_len+right_len;
    char *new_str = (char*)commonMalloc(new_len+1);
    new_str[new_len] = '\0';
    int i; for(i=0; i<left_len; i++) {
	new_str[i] = left[i];
    }
    for(int j=0; j<right_len; j++) {
	new_str[i+j] = right[j];
    }
    return new_str;
}
