/*
 * util.c - commonly used utility functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tigerc/Support/util.h>

void *checked_malloc(int len) {
  void *p = malloc(len);
  if (!p) {
    fprintf(stderr,"\nRan out of memory!\n");
    exit(1);
  }
  return p;
}

string String(const char *s) {
  string p = checked_malloc(strlen(s) + 1);
  strcpy(p, s);
  return p;
}

U_boolList U_BoolList(bool head, U_boolList tail) { 
  U_boolList list = checked_malloc(sizeof(*list));
  list->head = head;
  list->tail = tail;
  return list;
}
