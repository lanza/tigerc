#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>

#include <tigerc/Parse/Parser.h>
#include <tigerc/Lex/Lexer.h>
#include <tigerc/Interpreter/Interpreter.h>

#include "../Utils/testingUtils.h"

#include <gmock/gmock.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace testing; 

typedef tlang::Parser::token::yytokentype Token;
typedef tlang::Parser::symbol_type LexerOutput;

class LexerTest: public ::testing::Test {

public:
  LexerTest() { }
  void ScanString(std::string string) {
    std::stringstream* ss = new std::stringstream;
    ss->str(string);
    lexer = new tlang::Lexer(ss);
  }
  void ScanFile(std::string string) {
    std::ifstream* ifs = new std::ifstream(string);
    lexer = new tlang::Lexer(ifs);
  }
  Token GetToken() { return lexer->get_next_token().token(); } 
  LexerOutput GetOutput() { return lexer->get_next_token(); }
private:
  tlang::Lexer *lexer; 
};

#define TESTLEX(name) TEST_F(LexerTest, name)
#define AssertNextToken(name) ASSERT_THAT(GetToken(), Eq(name))
#define AssertNextTokenNot(name) ASSERT_THAT(GetToken(), Ne(name))

void AssertNextOutputInt(LexerOutput output, Token name, int value) {
  ASSERT_THAT(output.token(), Eq(name)); 
  ASSERT_THAT(output.value.as<int>(), Eq(value));
}

TESTLEX(ForStringReturnsForToken) {
  ScanString("for"); 
  AssertNextToken(Token::FOR);
}
TESTLEX(ForDoStringReturnsForTokenThenDoToken) {
  ScanString("for do");
  AssertNextToken(Token::FOR);
  AssertNextToken(Token::DO);
}

TESTLEX(OfBreakNilFunctionVarTypeMatchesTokens) {
  ScanString("of break nil function var type");
  AssertNextToken(Token::OF);
  AssertNextToken(Token::BREAK);
  AssertNextToken(Token::NIL);
  AssertNextToken(Token::FUNCTION);
  AssertNextToken(Token::VAR);
  AssertNextToken(Token::TYPE);
}

TESTLEX(AndOrAssignArrayIfThenElseWhileAllWork) {
  ScanString("and or assign array if then else while");
  AssertNextToken(Token::AND);
  AssertNextToken(Token::OR);
  AssertNextToken(Token::ASSIGN);
  AssertNextToken(Token::ARRAY);
  AssertNextToken(Token::IF);
  AssertNextToken(Token::THEN);
  AssertNextToken(Token::ELSE);
  AssertNextToken(Token::WHILE);
}

TESTLEX(NumberReturnsNumberToken) {
  ScanString("41234 4121512 44241 5555");
  for (int i = 0; i < 4; i++) {
    AssertNextToken(Token::INT);
  }
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(CommaReturnsCommaToken) {
  ScanString(", , , ");
  AssertNextToken(Token::COMMA);
  AssertNextToken(Token::COMMA);
  AssertNextToken(Token::COMMA);
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(JustSpacesReturnsNoTokensExceptEOF) {
  ScanString("    ");
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(IgnoresCommentsInLineOneOfTest1) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  AssertNextToken(Token::LET);
}

TESTLEX(IgnoresTabs) {
  ScanString("\tlet \tin");
  AssertNextToken(Token::LET);
  AssertNextToken(Token::IN);
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(ParsesEqualSign) {
  ScanString(" = = = ");
  AssertNextToken(Token::EQ);
  AssertNextToken(Token::EQ);
  AssertNextToken(Token::EQ);
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(ParsesAllLowercaseIDsProperly) {
  ScanString("\tmuffin = riley");
  AssertNextToken(Token::ID);
  AssertNextToken(Token::EQ);
  AssertNextToken(Token::ID);
}

TESTLEX(ParsesColonLRBrackets) {
  ScanString(" muffin : [10] :=");
  AssertNextToken(Token::ID);
  AssertNextToken(Token::COLON);
  AssertNextToken(Token::LBRACK);
  AssertNextToken(Token::INT);
  AssertNextToken(Token::RBRACK);
  AssertNextToken(Token::ASSIGN);
  AssertNextToken(Token::ENDOFFILE);
}


TESTLEX(Test1LexesProperly) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  AssertNextToken(Token::LET);
  AssertNextToken(Token::TYPE);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::EQ);
  AssertNextToken(Token::ARRAY);
  AssertNextToken(Token::OF);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::VAR);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::COLON);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::ASSIGN);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::LBRACK);
  AssertNextToken(Token::INT);
  AssertNextToken(Token::RBRACK);
  AssertNextToken(Token::OF);
  AssertNextToken(Token::INT);
  AssertNextToken(Token::IN);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::END);
}

TESTLEX(LexLRBrace) {
  ScanString(" { }");
  AssertNextToken(Token::LBRACE);
  AssertNextToken(Token::RBRACE);
}

TESTLEX(LexPeriod) {
  ScanString(" . muffin.age ");
  AssertNextToken(Token::DOT);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::DOT);
  AssertNextToken(Token::ID);
}
TESTLEX(LexSemicolon) {
  ScanString(" ; muffin;age ;");
  AssertNextToken(Token::SEMICOLON);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::SEMICOLON);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::SEMICOLON);
}
TESTLEX(LexMathematicalOperators) {
  ScanString("+ - * / != < > >= <=");
  AssertNextToken(Token::PLUS);
  AssertNextToken(Token::MINUS);
  AssertNextToken(Token::TIMES);
  AssertNextToken(Token::DIVIDE);
  AssertNextToken(Token::NEQ);
  AssertNextToken(Token::LT);
  AssertNextToken(Token::GT);
  AssertNextToken(Token::GE);
  AssertNextToken(Token::LE);
}

TESTLEX(LexString) {
  ScanString(" \" muffin is a dogadoogle \"");
  AssertNextToken(Token::STRING);
  AssertNextToken(Token::ENDOFFILE);
}

TESTLEX(TestFilesTwoAndThreeReturnNoIllegalTokens) {
  std::vector<std::string> files = {
    "test2.tig",
    "test3.tig",
    "test4.tig",
    "test5.tig",
    "test9.tig",
  };
  for (auto s : files) {
    auto f = PathFromFilename(s);
    ScanFile(f);
    Token t;
    while ((t = GetToken()) != Token::ENDOFFILE) 
      ASSERT_THAT(t, Ne(Token::ILLEGAL));
  }
}

TESTLEX(LexParens) {
  ScanString(" ( muffin dog )");
  AssertNextToken(Token::LPAREN);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::ID);
  AssertNextToken(Token::RPAREN);
}

/*
TESTLEX(TestNumbersAreStored) { 
  // if (10 > 20) then 30 else 40
  std::string filename = PathFromFilename("test8.tig");
  ScanFile(filename);

  LexerOutput Output;
  AssertNextToken(Token::IF);
  Output = GetOutput();
  AssertNextToken(Output, Token::LPAREN);
  AssertNextOutputInt(Token::INT, 10);
  AssertNextToken(Token::GT);
  AssertNextOutputInt(Token::INT, 20);
  AssertNextToken(Token::RPAREN);
  AssertNextToken(Token::THEN);
  AssertNextOutputInt(Token::INT, 30);
  AssertNextToken(Token::ELSE);
  AssertNextOutputInt(Token::INT, 40);
}


TESTLEX(TestNumbersAreBeingCheckedProperly) { 
  // if (10 > 20) then 30 else 40
  char *filename = PathFromFilename("test8.tig");
  ScanFile(filename);

  int token;

  token = GetToken(); a_tok_equal(IF);
  token = GetToken(); a_tok_equal(LPAREN);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Ne(40));
  token = GetToken(); a_tok_equal(GT);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Ne(30));
  token = GetToken(); a_tok_equal(RPAREN);
  token = GetToken(); a_tok_equal(THEN);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Ne(20));
  token = GetToken(); a_tok_equal(ELSE);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Ne(10)); 
}

#define TestLex(name) TESTLEX(name)

#define ExpectNextTokStr(tok,str) token = GetToken(); a_tok_equal(tok); \
                                          if (strlen(str) > 0) EXPECT_THAT(strcmp(yylval.sval,str), 0)
#define ExpectNextTok(tok) ExpectNextTokStr(tok,"")

#define StartFile(name) ScanFile(PathFromFilename(name))

TestLex(StringsAreStoredProperly) {
  */
  /* let
   *	 type rectype = {name:string, age:int}
   *	 var rec1:rectype := rectype {name="Nobody", age=1000}
   * in
   *	 rec1.name := "Somebody";
   *	 rec1
   * end
   */ 
  /*
  StartFile("test3.tig");

  int token;

  ExpectNextTok(LET);
  ExpectNextTok(TYPE);
  ExpectNextTokStr(ID, "rectype");
  ExpectNextTok(EQ);
  ExpectNextTok(LBRACE);
  ExpectNextTokStr(ID, "name");
  ExpectNextTok(COLON);
  ExpectNextTokStr(ID, "string");
  ExpectNextTok(COMMA);
  ExpectNextTokStr(ID, "age");
  ExpectNextTok(COLON);
  ExpectNextTokStr(ID, "int");
  ExpectNextTok(RBRACE);
}

*/













