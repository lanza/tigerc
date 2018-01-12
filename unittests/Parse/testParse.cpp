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
  yy_scan_string("let testID := 4");
  int token; 
  token = yylex(); EXPECT_THAT(token, Eq(LET));
  token = yylex(); EXPECT_THAT(token, Eq(ID)); EXPECT_THAT(strcmp(yylval.sval, "testID"), Eq(0));
  token = yylex(); EXPECT_THAT(token, Eq(ASSIGN));
  token = yylex(); EXPECT_THAT(token, Eq(INT)); EXPECT_THAT(yylval.ival, Eq(4)); 
}

#define ExpectParse() EXPECT_THAT(yyparse(), Eq(0))
#define ExpectFail() EXPECT_THAT(yyparse(), Ne(0))

TestParse(AcceptsVarDecls) {
  yy_scan_string("let var muffin := 4 in end"); 
  ExpectParse(); 
}

TestParse(AcceptsVarDeclsWithTypeId) {
  yy_scan_string("let var muffin : int := 4 in end");
  ExpectParse();
}

TestParse(AcceptsFuncDecl) {
  yy_scan_string("let function doSomething ( ) = 4 in end");
  ExpectParse();
}

TestParse(AcceptsTypeDecl) {
  yy_scan_string("let type person = int in end");
  ExpectParse();
}

TestParse(AcceptsTypeDeclWithArrayOf) {
  yy_scan_string("let type person = array of int in end");
  ExpectParse();
}

TestParse(AcceptsTypeDeclWithEmptyBraces) {
  yy_scan_string("let type person = { } in end");
  ExpectParse();
}
TestParse(AcceptsTypeDeclWithBracesContainingAIDTypeIdPair) {
  yy_scan_string("let type person = { name : string } in end");
  ExpectParse();
}
TestParse(AcceptsTypeDeclWithBracesContainingMultipleIDTypeIdPairs) {
  yy_scan_string("let type person = { name : string , age : int } in end");
  ExpectParse();
}

TestParse(AcceptsMultipleVarDecls) {
  yy_scan_string("let var muffin := 4 \n var riley := 2 in end");
  ExpectParse();
}
  
TestParse(AcceptsNil) {
  yy_scan_string("nil");
  ExpectParse();
}

TestParse(AcceptsFuncWithTyFields) {
  yy_scan_string("let function doSomething ( name : string , age : int ) = 4 in end");
  ExpectParse();
}

TestParse(AcceptsFuncWithTyFieldsAndTypeId) {
  yy_scan_string("let function doSomething ( name : string , age : int ) : int = 4 in end");
  ExpectParse();
}

TestParse(AcceptsTwoExpInParen) {
  yy_scan_string("(4; 32)");
  ExpectParse();
}

TestParse(AcceptsEmptyParens) {
  yy_scan_string("( )");
  ExpectParse();
}

TestParse(AcceptsEmptyLet) {
  yy_scan_string("let var muffin := 4 in end");
  ExpectParse();
}

TestParse(AcceptsPlainString) {
  yy_scan_string("\"muffin\"");
  ExpectParse();
  //TODO: add the escape characters
}

TestParse(AcceptsUnaryMinus) {
  yy_scan_string("-4");
  ExpectParse();
}

TestParse(AcceptsEmptyFunctionCall) {
  yy_scan_string("doSomething()");
  ExpectParse();
}

TestParse(AcceptsFunctionCallWithTwoArgs) {
  yy_scan_string("doSomething(3, 4)");
  ExpectParse();
}

TestParse(AcceptsAnExpListOfOps) {
  yy_scan_string("(4 + 4; 4 - 4; 4 * 4; 4 / 4; 4 != 4; 4 > 4; 4 >= 4; 4 < 4; 4 <= 4; 4 and 4; 4 or 4; 4 = 4; 4 <> 4 )");
  ExpectParse();
}

TestParse(AcceptsRecordCreation) {
  yy_scan_string("person { age = 24, name = \"Nathan\" }");
  ExpectParse();
}

TestParse(AcceptsTrickyRecordCreation) {
  yy_scan_string("person { age = ageo, name = nameo }");
  ExpectParse();
}



TestParse(AcceptsArrayInit) {
  yy_scan_string("int[4] of 2");
  ExpectParse();
} 

// dots and arrays
TestParse(AcceptsOneDot) {
  yy_scan_string("dog.muffin");
  ExpectParse();
}
TestParse(AcceptsOneDotAssign) {
  yy_scan_string("dog.muffin := 4");
  ExpectParse();
}
TestParse(AcceptsChainedDots) {
  yy_scan_string("dog.muffin.name.first");
  ExpectParse();
}
TestParse(AcceptsChainedDotsAssign) {
  yy_scan_string("dog.muffin.name.first := 4");
  ExpectParse();
}

TestParse(AcceptsOneArray) {
  yy_scan_string("muffin[0]");
  ExpectParse(); 
}

TestParse(AcceptsArrayAssign) {
  yy_scan_string("muffin[0] := 14");
  ExpectParse();
} 
TestParse(AcceptsChainedArray) {
  yy_scan_string("riley[0][0][0]");
  ExpectParse();
}
TestParse(AcceptsChainedArrayAssign) {
  yy_scan_string("riley[0][0][0] := 14");
  ExpectParse();
}
TestParse(AcceptsMixedArrayAndDots) {
  yy_scan_string("riley[0].muffin[3].belle[1][2].eric.kristin");
  ExpectParse();
}
TestParse(AcceptsMixedArrayAndDotsAssign) {
  yy_scan_string("riley[0].muffin[3].belle[1][2].eric.kristin := 22");
  ExpectParse();
}

TestParse(AcceptsMixedArrayAndDotsOfType) {
  yy_scan_string("riley[0].muffin[3].belle[1][2].eric.kristin[2] of int");
  ExpectFail();
}

TestParse(RejectsDotAssign) {
  yy_scan_string("muffin.dog of int");
  ExpectFail();
}

TestParse(RejectsIDAssign) {
  yy_scan_string("muffin of int");
  ExpectFail();
}

#define ExpectGood(string) yy_scan_string(string); ExpectParse()

TestParse(AcceptsIfThen) {
  ExpectGood("if 3 then 4");
}

TestParse(AcceptsIfThenElse) {
  ExpectGood("if 3 then 4 else 5");
}

TestParse(AcceptsWhileDo) {
  ExpectGood("while 3 do 4");
}

TestParse(AcceptsForExp) {
  ExpectGood("for num := 0 to 10 do 5");
}

TestParse(AcceptsBreakInFor) {
  ExpectGood("for num := 0 to 10 do break");
}

TestParse(AcceptsMultipleExpInLet) {
  ExpectGood("let var q := 4 in 3; 4; 5; 6 end");
}

TestParse(AcceptsExpInParen) {
  ExpectGood("(4)");
}

#define TestParseAcceptsTestCase(name) TEST(testParseTestCases,name) { \
  StartFile(#name".tig"); \
  ExpectParse(); \
}
#define TestParseRejectsTestCase(name) TEST(testParseTestCases,name) { \
  StartFile(#name".tig"); \
  ExpectFail(); \
}

TestParseAcceptsTestCase(merge);
TestParseAcceptsTestCase(queens); 
TestParseAcceptsTestCase(test1);
TestParseAcceptsTestCase(test2);
TestParseAcceptsTestCase(test3);
TestParseAcceptsTestCase(test4);
TestParseAcceptsTestCase(test5);
TestParseAcceptsTestCase(test6);
TestParseAcceptsTestCase(test7);
TestParseAcceptsTestCase(test8);
TestParseAcceptsTestCase(test9);
TestParseAcceptsTestCase(test10); 
TestParseAcceptsTestCase(test11);
TestParseAcceptsTestCase(test12);
TestParseAcceptsTestCase(test13);
TestParseAcceptsTestCase(test14);
TestParseAcceptsTestCase(test15);
TestParseAcceptsTestCase(test16);
TestParseAcceptsTestCase(test17);
TestParseAcceptsTestCase(test18);
TestParseAcceptsTestCase(test19);
TestParseAcceptsTestCase(test20);
TestParseAcceptsTestCase(test21);
TestParseAcceptsTestCase(test22);
TestParseAcceptsTestCase(test23);
TestParseAcceptsTestCase(test24);
TestParseAcceptsTestCase(test25);
TestParseAcceptsTestCase(test26);
TestParseAcceptsTestCase(test27);
TestParseAcceptsTestCase(test28);
TestParseAcceptsTestCase(test29);
TestParseAcceptsTestCase(test30);
TestParseAcceptsTestCase(test31);
TestParseAcceptsTestCase(test32);
TestParseAcceptsTestCase(test33);
TestParseAcceptsTestCase(test34);
TestParseAcceptsTestCase(test35);
TestParseAcceptsTestCase(test36);
TestParseAcceptsTestCase(test37);
TestParseAcceptsTestCase(test38);
TestParseAcceptsTestCase(test39);
TestParseAcceptsTestCase(test40);
TestParseAcceptsTestCase(test41);
TestParseAcceptsTestCase(test42);
TestParseAcceptsTestCase(test43);
TestParseAcceptsTestCase(test44);
TestParseAcceptsTestCase(test45);
TestParseAcceptsTestCase(test46);
TestParseAcceptsTestCase(test47);
TestParseAcceptsTestCase(test48);
TestParseRejectsTestCase(test49);











