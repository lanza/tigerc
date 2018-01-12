%{
#include <stdio.h>
#include <tigerc/Support/util.h>
#include <tigerc/Support/errormsg.h>

int yylex();
void yyerror(char *s) {
  EM_error(EM_tokPos, "%s", s);
}
%}

%union {
  int pos;
  int ival;
  string sval;
}

%token <sval> ID STRING
%token <ival> INT

%token
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
  LBRACE RBRACE DOT
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF
  BREAK NIL
  FUNCTION VAR TYPE
  ILLEGAL

%start program

%%

program: exp 
       ;

exp: LET ID ASSIGN INT 
   ;

%%
