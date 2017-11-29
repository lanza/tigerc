#ifndef util_h
#define util_h

#include <assert.h>

typedef char *string;
#ifndef __CPP__
typedef char bool;
#endif

#define TRUE 1
#define FALSE 0

void *checked_malloc(int);
string String(const char *);

typedef struct U_boolList_ *U_boolList;
struct U_boolList_ {bool head; U_boolList tail;};
U_boolList U_BoolList(bool head, U_boolList tail);

#endif /* util_h */
