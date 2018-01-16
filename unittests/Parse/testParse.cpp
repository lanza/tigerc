
#include <gmock/gmock.h>
#include "../Utils/testingUtils.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

extern int yyparse();
extern int yydebug; 

using namespace testing;

class ParserTest: public ::testing::Test {

public:
  ParserTest() {
    lexer = new yyFlexLexer;
  }
  void ScanString(std::string string) {
    std::stringstream ss;
    ss.str(string);
    lexer->yyrestart(&ss); 
  }
  void ScanFile(std::string string) {
    std::ifstream ifs(string);
    lexer->yyrestart(&ifs); 
  }
  int GetToken() { return lexer->yylex(); } 
private:
  FlexLexer *lexer; 
};

#define ParserTest(name) TEST_F(ParserTest, name)

ParserTest(LexingStillWorks) {
  ScanString("let testID := 4");
  int token; 
  token = GetToken(); EXPECT_THAT(token, Eq(LET));
  token = GetToken(); EXPECT_THAT(token, Eq(ID)); EXPECT_THAT(strcmp(yylval.sval, "testID"), Eq(0));
  token = GetToken(); EXPECT_THAT(token, Eq(ASSIGN));
  token = GetToken(); EXPECT_THAT(token, Eq(INT)); EXPECT_THAT(yylval.ival, Eq(4)); 
}

#define ExpectParse() EXPECT_THAT(yyparse(), Eq(0))
#define ExpectFail() EXPECT_THAT(yyparse(), Ne(0))

ParserTest(AcceptsVarDecls) {
  ScanString("let var muffin := 4 in end"); 
  ExpectParse(); 
}

ParserTest(AcceptsVarDeclsWithTypeId) {
  ScanString("let var muffin : int := 4 in end");
  ExpectParse();
}

ParserTest(AcceptsFuncDecl) {
  ScanString("let function doSomething ( ) = 4 in end");
  ExpectParse();
}

ParserTest(AcceptsTypeDecl) {
  ScanString("let type person = int in end");
  ExpectParse();
}

ParserTest(AcceptsTypeDeclWithArrayOf) {
  ScanString("let type person = array of int in end");
  ExpectParse();
}

ParserTest(AcceptsTypeDeclWithEmptyBraces) {
  ScanString("let type person = { } in end");
  ExpectParse();
}
ParserTest(AcceptsTypeDeclWithBracesContainingAIDTypeIdPair) {
  ScanString("let type person = { name : string } in end");
  ExpectParse();
}
ParserTest(AcceptsTypeDeclWithBracesContainingMultipleIDTypeIdPairs) {
  ScanString("let type person = { name : string , age : int } in end");
  ExpectParse();
}

ParserTest(AcceptsMultipleVarDecls) {
  ScanString("let var muffin := 4 \n var riley := 2 in end");
  ExpectParse();
}
  
ParserTest(AcceptsNil) {
  ScanString("nil");
  ExpectParse();
}

ParserTest(AcceptsFuncWithTyFields) {
  ScanString("let function doSomething ( name : string , age : int ) = 4 in end");
  ExpectParse();
}

ParserTest(AcceptsFuncWithTyFieldsAndTypeId) {
  ScanString("let function doSomething ( name : string , age : int ) : int = 4 in end");
  ExpectParse();
}

ParserTest(AcceptsTwoExpInParen) {
  ScanString("(4; 32)");
  ExpectParse();
}

ParserTest(AcceptsEmptyParens) {
  ScanString("( )");
  ExpectParse();
}

ParserTest(AcceptsEmptyLet) {
  ScanString("let var muffin := 4 in end");
  ExpectParse();
}

ParserTest(AcceptsPlainString) {
  ScanString("\"muffin\"");
  ExpectParse();
  //TODO: add the escape characters
}

ParserTest(AcceptsUnaryMinus) {
  ScanString("-4");
  ExpectParse();
}

ParserTest(AcceptsEmptyFunctionCall) {
  ScanString("doSomething()");
  ExpectParse();
}

ParserTest(AcceptsFunctionCallWithTwoArgs) {
  ScanString("doSomething(3, 4)");
  ExpectParse();
}

ParserTest(AcceptsAnExpListOfOps) {
  ScanString("(4 + 4; 4 - 4; 4 * 4; 4 / 4; 4 != 4; 4 > 4; 4 >= 4; 4 < 4; 4 <= 4; 4 and 4; 4 or 4; 4 = 4; 4 <> 4 )");
  ExpectParse();
}

ParserTest(AcceptsRecordCreation) {
  ScanString("person { age = 24, name = \"Nathan\" }");
  ExpectParse();
}

ParserTest(AcceptsTrickyRecordCreation) {
  ScanString("person { age = ageo, name = nameo }");
  ExpectParse();
}



ParserTest(AcceptsArrayInit) {
  ScanString("int[4] of 2");
  ExpectParse();
} 

// dots and arrays
ParserTest(AcceptsOneDot) {
  ScanString("dog.muffin");
  ExpectParse();
}
ParserTest(AcceptsOneDotAssign) {
  ScanString("dog.muffin := 4");
  ExpectParse();
}
ParserTest(AcceptsChainedDots) {
  ScanString("dog.muffin.name.first");
  ExpectParse();
}
ParserTest(AcceptsChainedDotsAssign) {
  ScanString("dog.muffin.name.first := 4");
  ExpectParse();
}

ParserTest(AcceptsOneArray) {
  ScanString("muffin[0]");
  ExpectParse(); 
}

ParserTest(AcceptsArrayAssign) {
  ScanString("muffin[0] := 14");
  ExpectParse();
} 
ParserTest(AcceptsChainedArray) {
  ScanString("riley[0][0][0]");
  ExpectParse();
}
ParserTest(AcceptsChainedArrayAssign) {
  ScanString("riley[0][0][0] := 14");
  ExpectParse();
}
ParserTest(AcceptsMixedArrayAndDots) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin");
  ExpectParse();
}
ParserTest(AcceptsMixedArrayAndDotsAssign) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin := 22");
  ExpectParse();
}

ParserTest(AcceptsMixedArrayAndDotsOfType) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin[2] of int");
  ExpectFail();
}

ParserTest(RejectsDotAssign) {
  ScanString("muffin.dog of int");
  ExpectFail();
}

ParserTest(RejectsIDAssign) {
  ScanString("muffin of int");
  ExpectFail();
}

#define ExpectGood(string) ScanString(string); ExpectParse()

ParserTest(AcceptsIfThen) {
  ExpectGood("if 3 then 4");
}

ParserTest(AcceptsIfThenElse) {
  ExpectGood("if 3 then 4 else 5");
}

ParserTest(AcceptsWhileDo) {
  ExpectGood("while 3 do 4");
}

ParserTest(AcceptsForExp) {
  ExpectGood("for num := 0 to 10 do 5");
}

ParserTest(AcceptsBreakInFor) {
  ExpectGood("for num := 0 to 10 do break");
}

ParserTest(AcceptsMultipleExpInLet) {
  ExpectGood("let var q := 4 in 3; 4; 5; 6 end");
}

ParserTest(AcceptsExpInParen) {
  ExpectGood("(4)");
}

#define StartFile(name) ScanFile(PathFromFilename(name))

#define ParserTestAcceptsTestCase(name) ParserTest(name) { \
  StartFile(#name".tig"); \
  ExpectParse(); \
}
#define ParserTestRejectsTestCase(name) ParserTest(name) { \
  StartFile(#name".tig"); \
  ExpectFail(); \
}

ParserTestAcceptsTestCase(merge);
ParserTestAcceptsTestCase(queens); 
ParserTestAcceptsTestCase(test1);
ParserTestAcceptsTestCase(test2);
ParserTestAcceptsTestCase(test3);
ParserTestAcceptsTestCase(test4);
ParserTestAcceptsTestCase(test5);
ParserTestAcceptsTestCase(test6);
ParserTestAcceptsTestCase(test7);
ParserTestAcceptsTestCase(test8);
ParserTestAcceptsTestCase(test9);
ParserTestAcceptsTestCase(test10); 
ParserTestAcceptsTestCase(test11);
ParserTestAcceptsTestCase(test12);
ParserTestAcceptsTestCase(test13);
ParserTestAcceptsTestCase(test14);
ParserTestAcceptsTestCase(test15);
ParserTestAcceptsTestCase(test16);
ParserTestAcceptsTestCase(test17);
ParserTestAcceptsTestCase(test18);
ParserTestAcceptsTestCase(test19);
ParserTestAcceptsTestCase(test20);
ParserTestAcceptsTestCase(test21);
ParserTestAcceptsTestCase(test22);
ParserTestAcceptsTestCase(test23);
ParserTestAcceptsTestCase(test24);
ParserTestAcceptsTestCase(test25);
ParserTestAcceptsTestCase(test26);
ParserTestAcceptsTestCase(test27);
ParserTestAcceptsTestCase(test28);
ParserTestAcceptsTestCase(test29);
ParserTestAcceptsTestCase(test30);
ParserTestAcceptsTestCase(test31);
ParserTestAcceptsTestCase(test32);
ParserTestAcceptsTestCase(test33);
ParserTestAcceptsTestCase(test34);
ParserTestAcceptsTestCase(test35);
ParserTestAcceptsTestCase(test36);
ParserTestAcceptsTestCase(test37);
ParserTestAcceptsTestCase(test38);
ParserTestAcceptsTestCase(test39);
ParserTestAcceptsTestCase(test40);
ParserTestAcceptsTestCase(test41);
ParserTestAcceptsTestCase(test42);
ParserTestAcceptsTestCase(test43);
ParserTestAcceptsTestCase(test44);
ParserTestAcceptsTestCase(test45);
ParserTestAcceptsTestCase(test46);
ParserTestAcceptsTestCase(test47);
ParserTestAcceptsTestCase(test48);
ParserTestRejectsTestCase(test49);











