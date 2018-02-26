#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>

#include <tigerc/Parse/Parser.h>
#include <tigerc/Lex/Lexer.h>
#include <tigerc/Interpreter/Interpreter.h>

#include "../Utils/testingUtils.h"

#include <gmock/gmock.h>

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

using namespace testing;

typedef tlang::Parser::token::yytokentype Token;
typedef tlang::Parser::symbol_type LexerOutput;

class TestParser: public ::testing::Test {

public:
  TestParser() { }
  void ScanString(std::string string) {
    std::stringstream* ss = new std::stringstream;
    ss->str(string);
    lexer = new tlang::Lexer(ss);
    parser = new tlang::Parser(*lexer);
  }
  void ScanFile(std::string string) {
    std::ifstream* ifs = new std::ifstream(string);
    lexer = new tlang::Lexer(ifs); 
    parser = new tlang::Parser(*lexer);
  }
  Token GetToken() { return lexer->get_next_token().token(); }
  LexerOutput GetOutput() { return lexer->get_next_token(); }

  friend class tlang::Parser;
  tlang::Lexer *lexer; 
  tlang::Parser *parser;
};

#define TestParser(name) TEST_F(TestParser, name)
#define AssertNextToken(name) ASSERT_THAT(GetToken(), Eq(name))
#define AssertNextTokenNot(name) ASSERT_THAT(GetToken(), Ne(name))

#define ExpectSuccess() EXPECT_THAT(parser->parse(), Eq(0))
#define ExpectFail() EXPECT_THAT(parser->parse(), Ne(0))



TestParser(AcceptsVarDecls) {
  ScanString("let var muffin := 4 in end"); 
  ExpectSuccess(); 
}

TestParser(AcceptsVarDeclsWithTypeId) {
  ScanString("let var muffin : int := 4 in end");
  ExpectSuccess();
}

TestParser(AcceptsFuncDecl) {
  ScanString("let function doSomething ( ) = 4 in end");
  ExpectSuccess();
}

TestParser(AcceptsTypeDecl) {
  ScanString("let type person = int in end");
  ExpectSuccess();
}

TestParser(AcceptsTypeDeclWithArrayOf) {
  ScanString("let type person = array of int in end");
  ExpectSuccess();
}

TestParser(AcceptsTypeDeclWithEmptyBraces) {
  ScanString("let type person = { } in end");
  ExpectSuccess();
}
TestParser(AcceptsTypeDeclWithBracesContainingAIDTypeIdPair) {
  ScanString("let type person = { name : string } in end");
  ExpectSuccess();
}
TestParser(AcceptsTypeDeclWithBracesContainingMultipleIDTypeIdPairs) {
  ScanString("let type person = { name : string , age : int } in end");
  ExpectSuccess();
}

TestParser(AcceptsMultipleVarDecls) {
  ScanString("let var muffin := 4 \n var riley := 2 in end");
  ExpectSuccess();
}
  
TestParser(AcceptsNil) {
  ScanString("nil");
  ExpectSuccess();
}

TestParser(AcceptsFuncWithTyFields) {
  ScanString("let function doSomething ( name : string , age : int ) = 4 in end");
  ExpectSuccess();
}

TestParser(AcceptsFuncWithTyFieldsAndTypeId) {
  ScanString("let function doSomething ( name : string , age : int ) : int = 4 in end");
  ExpectSuccess();
}

TestParser(AcceptsTwoExpInParen) {
  ScanString("(4; 32)");
  ExpectSuccess();
}

TestParser(AcceptsEmptyParens) {
  ScanString("( )");
  ExpectSuccess();
}

TestParser(AcceptsEmptyLet) {
  ScanString("let var muffin := 4 in end");
  ExpectSuccess();
}

TestParser(AcceptsPlainString) {
  ScanString("\"muffin\"");
  ExpectSuccess();
  //TODO: add the escape characters
}

TestParser(AcceptsUnaryMinus) {
  ScanString("-4");
  ExpectSuccess();
}

TestParser(AcceptsEmptyFunctionCall) {
  ScanString("doSomething()");
  ExpectSuccess();
}

TestParser(AcceptsFunctionCallWithTwoArgs) {
  ScanString("doSomething(3, 4)");
  ExpectSuccess();
}

TestParser(AcceptsAnExpListOfOps) {
  ScanString("(4 + 4; 4 - 4; 4 * 4; 4 / 4; 4 != 4; 4 > 4; 4 >= 4; 4 < 4; 4 <= 4; 4 and 4; 4 or 4; 4 = 4; 4 <> 4 )");
  ExpectSuccess();
}

TestParser(AcceptsRecordCreation) {
  ScanString("person { age = 24, name = \"Nathan\" }");
  ExpectSuccess();
}

TestParser(AcceptsTrickyRecordCreation) {
  ScanString("person { age = ageo, name = nameo }");
  ExpectSuccess();
}

TestParser(AcceptsArrayInit) {
  ScanString("int[4] of 2");
  ExpectSuccess();
} 

// dots and arrays
TestParser(AcceptsOneDot) {
  ScanString("dog.muffin");
  ExpectSuccess();
}
TestParser(AcceptsOneDotAssign) {
  ScanString("dog.muffin := 4");
  ExpectSuccess();
}
TestParser(AcceptsChainedDots) {
  ScanString("dog.muffin.name.first");
  ExpectSuccess();
}
TestParser(AcceptsChainedDotsAssign) {
  ScanString("dog.muffin.name.first := 4");
  ExpectSuccess();
}

TestParser(AcceptsOneArray) {
  ScanString("muffin[0]");
  ExpectSuccess(); 
}

TestParser(AcceptsArrayAssign) {
  ScanString("muffin[0] := 14");
  ExpectSuccess();
} 
TestParser(AcceptsChainedArray) {
  ScanString("riley[0][0][0]");
  ExpectSuccess();
}
TestParser(AcceptsChainedArrayAssign) {
  ScanString("riley[0][0][0] := 14");
  ExpectSuccess();
}
TestParser(AcceptsMixedArrayAndDots) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin");
  ExpectSuccess();
}
TestParser(AcceptsMixedArrayAndDotsAssign) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin := 22");
  ExpectSuccess();
}

TestParser(AcceptsMixedArrayAndDotsOfType) {
  ScanString("riley[0].muffin[3].belle[1][2].eric.kristin[2] of int");
  ExpectFail();
}

TestParser(RejectsDotAssign) {
  ScanString("muffin.dog of int");
  ExpectFail();
}

TestParser(RejectsIDAssign) {
  ScanString("muffin of int");
  ExpectFail();
}

#define ExpectGood(string) ScanString(string); ExpectSuccess()

TestParser(AcceptsIfThen) {
  ExpectGood("if 3 then 4");
}

TestParser(AcceptsIfThenElse) {
  ExpectGood("if 3 then 4 else 5");
}

TestParser(AcceptsWhileDo) {
  ExpectGood("while 3 do 4");
}

TestParser(AcceptsForExp) {
  ExpectGood("for num := 0 to 10 do 5");
}

TestParser(AcceptsBreakInFor) {
  ExpectGood("for num := 0 to 10 do break");
}

TestParser(AcceptsMultipleExpInLet) {
  ExpectGood("let var q := 4 in 3; 4; 5; 6 end");
}

TestParser(AcceptsExpInParen) {
  ExpectGood("(4)");
}
#define StartFile(name) ScanFile(PathFromFilename(name))

#define TestParserAcceptsTestCase(name) TestParser(name) { \
  std::string q = PathFromFilename(#name".tig"); \
  ScanFile(q); \
  ExpectSuccess(); \
}
#define TestParserRejectsTestCase(name) TestParser(name) { \
  StartFile(#name".tig"); \
  ExpectFail(); \
}

TestParserAcceptsTestCase(merge);
TestParserAcceptsTestCase(queens); 
TestParserAcceptsTestCase(test1);
TestParserAcceptsTestCase(test2);
TestParserAcceptsTestCase(test3);
TestParserAcceptsTestCase(test4);
TestParserAcceptsTestCase(test5);
TestParserAcceptsTestCase(test6);
TestParserAcceptsTestCase(test7);
TestParserAcceptsTestCase(test8);
TestParserAcceptsTestCase(test9);
TestParserAcceptsTestCase(test10); 
TestParserAcceptsTestCase(test11);
TestParserAcceptsTestCase(test12);
TestParserAcceptsTestCase(test13);
TestParserAcceptsTestCase(test14);
TestParserAcceptsTestCase(test15);
TestParserAcceptsTestCase(test16);
TestParserAcceptsTestCase(test17);
TestParserAcceptsTestCase(test18);
TestParserAcceptsTestCase(test19);
TestParserAcceptsTestCase(test20);
TestParserAcceptsTestCase(test21);
TestParserAcceptsTestCase(test22);
TestParserAcceptsTestCase(test23);
TestParserAcceptsTestCase(test24);
TestParserAcceptsTestCase(test25);
TestParserAcceptsTestCase(test26);
TestParserAcceptsTestCase(test27);
TestParserAcceptsTestCase(test28);
TestParserAcceptsTestCase(test29);
TestParserAcceptsTestCase(test30);
TestParserAcceptsTestCase(test31);
TestParserAcceptsTestCase(test32);
TestParserAcceptsTestCase(test33);
TestParserAcceptsTestCase(test34);
TestParserAcceptsTestCase(test35);
TestParserAcceptsTestCase(test36);
TestParserAcceptsTestCase(test37);
TestParserAcceptsTestCase(test38);
TestParserAcceptsTestCase(test39);
TestParserAcceptsTestCase(test40);
TestParserAcceptsTestCase(test41);
TestParserAcceptsTestCase(test42);
TestParserAcceptsTestCase(test43);
TestParserAcceptsTestCase(test44);
TestParserAcceptsTestCase(test45);
TestParserAcceptsTestCase(test46);
TestParserAcceptsTestCase(test47);
TestParserAcceptsTestCase(test48);
TestParserRejectsTestCase(test49);
