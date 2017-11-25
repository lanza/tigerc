#include <gmock/gmock.h>
extern "C" {
#include <tiger-lex.h>
}

using namespace testing; 

TEST(testLex, HelloWorld) {
  ASSERT_THAT(1, Eq(1));
};

TEST(testLex, LexHelloWorld) {
  int output = yylex();
}
