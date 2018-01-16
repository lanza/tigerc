#ifndef util_h
#define util_h

#include <cassert>

void *checked_malloc(int);
char *String(const char *);

typedef struct U_boolList_ *U_boolList;
struct U_boolList_ {
  bool head; 
  U_boolList tail;
};
U_boolList U_BoolList(bool head, U_boolList tail);

#endif /* util_h */
