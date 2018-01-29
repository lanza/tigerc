#include "testingUtils.h"

std::string PathFromFilename(std::string filename) {
  std::string prefix = "/Users/lanza/Projects/tigerc/unittests/testcases/";
  std::string full = prefix + filename;
  return full;
}
