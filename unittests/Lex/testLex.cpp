#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>
#include <tigerc/Parse/Parser.h>
#include <gmock/gmock.h>
#include <FlexLexer.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

extern YYSTYPE yylval;

using namespace testing; 

class LexerTest: public ::testing::Test {

public:
  LexerTest() {
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

#define TESTLEX(name) TEST_F(LexerTest, name)

TESTLEX(ForStringReturnsForToken) {
  ScanString("for");
  int output = GetToken();
  ASSERT_THAT(output, Eq(FOR));
}

TESTLEX(ForDoStringReturnsForTokenThenDoToken) {
  ScanString("for do");
  int output = GetToken();
  ASSERT_THAT(output, Eq(FOR));
  output = GetToken();
  ASSERT_THAT(output, Eq(DO)); 
}

TESTLEX(OfBreakNilFunctionVarTypeMatchesTokens) {
  ScanString("of break nil function var type");
  int output = GetToken();
  ASSERT_THAT(output, Eq(OF));
  output = GetToken();
  ASSERT_THAT(output, Eq(BREAK)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(NIL)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(FUNCTION)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(VAR)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(TYPE)); 
}

TESTLEX(AndOrAssignArrayIfThenElseWhileAllWork) {
  ScanString("and or assign array if then else while");
  int output = GetToken();
  ASSERT_THAT(output, Eq(AND));
  output = GetToken();
  ASSERT_THAT(output, Eq(OR)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(ASSIGN)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(ARRAY)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(IF)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(THEN)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(ELSE)); 
  output = GetToken();
  ASSERT_THAT(output, Eq(WHILE)); 
}

TESTLEX(NumberReturnsNumberToken) {
  ScanString("41234 4121512 44241 5555");
  for (int i = 0; i < 4; i++) {
    int token = GetToken();
    ASSERT_THAT(token, Eq(INT));
  }
}

TESTLEX(CommaReturnsCommaToken) {
  ScanString(", , , ");
  int token;
  int count = 0;
  while ((token = GetToken())) {
    ASSERT_THAT(token, Eq(COMMA));
    count++;
  }
  ASSERT_THAT(count, Eq(3));
}

TESTLEX(JustSpacesReturnsNoTokensExceptEOF) {
  ScanString("    ");
  int token = GetToken();
  ASSERT_THAT(token, Eq(0));
}

TESTLEX(LoadingAFileWorksUsingTestFileDotTig) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/Lex/testFile.tig");
  int token;
  token = GetToken();
  ASSERT_THAT(token, Eq(IF));
  token = GetToken();
  ASSERT_THAT(token, Eq(LET));
  token = GetToken();
  ASSERT_THAT(token, Eq(COMMA));
}


TESTLEX(IgnoresCommentsInLineOneOfTest1) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  int token = GetToken();
  ASSERT_THAT(token, Eq(LET)); 
}

TESTLEX(IgnoresTabs) {
  ScanString("\tlet \tin");
  int token = GetToken();
  ASSERT_THAT(token, Eq(LET));
  token = GetToken();
  ASSERT_THAT(token, Eq(IN));
}

TESTLEX(ParsesEqualSign) {
  ScanString(" = = = ");
  for (int i = 0; i < 3; i++) {
    int token = GetToken();
    ASSERT_THAT(token, Eq(EQ));
  }
  int token = GetToken();
  ASSERT_THAT(token, Eq(0));
}

TESTLEX(ParsesAllLowercaseIDsProperly) {
  ScanString("\tmuffin = riley");
  int token = GetToken();
  ASSERT_THAT(token, Eq(ID));
  token = GetToken();
  ASSERT_THAT(token, Eq(EQ));
  token = GetToken();
  ASSERT_THAT(token, Eq(ID));
  token = GetToken();
  ASSERT_THAT(token, Eq(0));
}

TESTLEX(ParsesColonLRBrackets) {
  ScanString(" muffin : [10] :=");
  int token;
  token = GetToken();
  ASSERT_THAT(token, Eq(ID));
  token = GetToken();
  ASSERT_THAT(token, Eq(COLON));
  token = GetToken();
  ASSERT_THAT(token, Eq(LBRACK));
  token = GetToken();
  ASSERT_THAT(token, Eq(INT));
  token = GetToken();
  ASSERT_THAT(token, Eq(RBRACK));
  token = GetToken();
  ASSERT_THAT(token, Eq(ASSIGN));
  token = GetToken();
  ASSERT_THAT(token, Eq(0));
}


TESTLEX(Test1LexesProperly) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  int token; 
  int tokens[] = { LET, TYPE, ID, EQ, ARRAY, OF, ID, VAR, ID, COLON, ID, ASSIGN, ID, LBRACK, INT, RBRACK, OF, INT, IN, ID, END };
  int index = 0;
  while ((token = GetToken())) {
    ASSERT_THAT(token, Eq(tokens[index]));
    index++;
  }
}

#define a_tok_equal(x) ASSERT_THAT(token, Eq(x));
#define a_tok_nequal(x) ASSERT_THAT(token, Ne(x));

TESTLEX(LexLRBrace) {
  ScanString(" { }");
  int token;
  token = GetToken();
  a_tok_equal(LBRACE);
  token = GetToken();
  a_tok_equal(RBRACE);
}

TESTLEX(LexPeriod) {
  ScanString(" . muffin.age ");
  int token;
  token = GetToken();
  a_tok_equal(DOT);
  token = GetToken();
  a_tok_equal(ID);
  token = GetToken();
  a_tok_equal(DOT);
  token = GetToken();
  a_tok_equal(ID);
}
TESTLEX(LexSemicolon) {
  ScanString(" ; muffin;age ;");
  int token;
  token = GetToken();
  a_tok_equal(SEMICOLON);
  token = GetToken();
  a_tok_equal(ID);
  token = GetToken();
  a_tok_equal(SEMICOLON);
  token = GetToken();
  a_tok_equal(ID);
  token = GetToken();
  a_tok_equal(SEMICOLON);
}
TESTLEX(LexMathematicalOperators) {
  ScanString("+ - * / != < > >= <=");
  int token;
  token = GetToken();
  a_tok_equal(PLUS);
  token = GetToken();
  a_tok_equal(MINUS);
  token = GetToken();
  a_tok_equal(TIMES);
  token = GetToken();
  a_tok_equal(DIVIDE);
  token = GetToken();
  a_tok_equal(NEQ);
  token = GetToken();
  a_tok_equal(LT);
  token = GetToken();
  a_tok_equal(GT);
  token = GetToken();
  a_tok_equal(GE); 
  token = GetToken();
  a_tok_equal(LE); 
}

TESTLEX(LexString) {
  ScanString(" \" muffin is a dogadoogle \"");
  int token;
  token = GetToken();
  a_tok_equal(STRING);
  token = GetToken();
  a_tok_equal(0);
}

TESTLEX(TestFileTwoReturnsNoIllegalTokens) {
  const int SIZE = 1;
  const char *files[SIZE] = {
    "/Users/lanza/Projects/tigerc/unittests/testcases/test2.tig",
  };
  for (int i = 0; i < SIZE; i++) {
    ScanFile(files[i]);
    int token;

    while ((token = GetToken())) {
      ASSERT_THAT(token, Ne(666));
    }
  }
}

TESTLEX(TestFileThreeReturnsNoIllegalTokens) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test3.tig");
  int token;

  while ((token = GetToken())) {
    ASSERT_THAT(token, Ne(666));
  }
}

TESTLEX(LexParens) {
  ScanString(" ( muffin dog )");
  int token;
  token = GetToken();
  a_tok_equal(LPAREN);
  token = GetToken();
  a_tok_equal(ID);
  token = GetToken();
  a_tok_equal(ID);
  token = GetToken();
  a_tok_equal(RPAREN);
  token = GetToken();
  a_tok_equal(0);
}


TESTLEX(TestFileFourReturnsNoIllegalTokens) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test4.tig");
  int token;

  while ((token = GetToken())) {
    ASSERT_THAT(token, Ne(666));
  } 
}


TESTLEX(TestFileFiveReturnsNoIllegalTokens) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test5.tig");
  int token;

  while ((token = GetToken())) {
    ASSERT_THAT(token, Ne(666));
  } 
}

TESTLEX(TestFileNineReturnsNoIllegalTokens) {
  ScanFile("/Users/lanza/Projects/tigerc/unittests/testcases/test9.tig");
  int token;

  while ((token = GetToken())) {
    ASSERT_THAT(token, Ne(666));
  } 
}

char *PathFromFilename(const char *filename) {
  static const char *prefix = "/Users/lanza/Projects/tigerc/unittests/testcases/";
  char *dest = (char *)checked_malloc(strlen(prefix) + strlen(filename) + 1);
  strcpy(dest, prefix);
  strcat(dest, filename);
  return dest; 
}

TEST(testTestingUtils, PathFromFilenameWorks) {
  char *filename = PathFromFilename("test9.tig"); 
  ASSERT_THAT(strcmp(filename, "/Users/lanza/Projects/tigerc/unittests/testcases/test9.tig"), 0);
} 

TESTLEX(TestNumbersAreStored) { 
  // if (10 > 20) then 30 else 40
  char *filename = PathFromFilename("test8.tig");
  ScanFile(filename);

  int token;

  token = GetToken(); a_tok_equal(IF);
  token = GetToken(); a_tok_equal(LPAREN);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Eq(10));
  token = GetToken(); a_tok_equal(GT);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Eq(20));
  token = GetToken(); a_tok_equal(RPAREN);
  token = GetToken(); a_tok_equal(THEN);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Eq(30));
  token = GetToken(); a_tok_equal(ELSE);
  token = GetToken(); a_tok_equal(INT); ASSERT_THAT(yylval.ival, Eq(40)); 
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
  /* let
   *	 type rectype = {name:string, age:int}
   *	 var rec1:rectype := rectype {name="Nobody", age=1000}
   * in
   *	 rec1.name := "Somebody";
   *	 rec1
   * end
   */ 
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















