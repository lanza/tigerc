#include "testingUtils.h"

char *PathFromFilename(const char *filename) {
  static const char *prefix = "/Users/lanza/Projects/tigerc/unittests/testcases/";
  char *dest = (char *)checked_malloc(strlen(prefix) + strlen(filename) + 1);
  strcpy(dest, prefix);
  strcat(dest, filename);
  return dest; 
}

