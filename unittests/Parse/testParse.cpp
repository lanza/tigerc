#define __CPP__

#include <gmock/gmock.h>
#include "../TestingUtils/testingUtils.h"
extern "C" {
extern int yyparse();
extern int yydebug;
}


using namespace testing;


#define TestParse(name) TEST(testParse, name)

TestParse(LexingStillWorks) {
  StartFile("simpletest1.tig");

  int token;

  token = yylex(); EXPECT_THAT(token, Eq(LET));
  token = yylex(); EXPECT_THAT(token, Eq(ID)); EXPECT_THAT(strcmp(yylval.sval, "testID"), Eq(0));
  token = yylex(); EXPECT_THAT(token, Eq(ASSIGN));
  token = yylex(); EXPECT_THAT(token, Eq(INT)); EXPECT_THAT(yylval.ival, Eq(4)); 
}

TestParse(SimpleTestOneyyparseReturnsZero) {
  StartFile("simpletest1.tig");

  EXPECT_THAT(yyparse(), Eq(0));
}
