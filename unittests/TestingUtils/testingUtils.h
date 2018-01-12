#include <gmock/gmock.h>
extern "C" {
#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h> 
#include <lex.h>
#include <parse.h>
}


string PathFromFilename(const char *filename) {
  static const char *prefix = "/Users/lanza/Projects/tigerc/unittests/testcases/";
  char *dest = (char *)checked_malloc(strlen(prefix) + strlen(filename) + 1);
  strcpy(dest, prefix);
  strcat(dest, filename);
  return dest; 
}

void StartFile(const char *filename) {
  string fn = PathFromFilename(filename);
  FILE *f = fopen(fn, "r");
  yyrestart(f);
}
