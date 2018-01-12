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

%right ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE 
%left PLUS MINUS
%left TIMES DIVIDE

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

decls: 
     | decl decls
     ;

decl: vardecl
    | funcdecl
    | typedecl
    ;

typedecl: TYPE typeid EQ ty
        ;

typeid: ID
      ;


ty: typeid
  | LBRACE tyfield RBRACE
  | ARRAY OF typeid
  ;

tyfield:
       | tyfieldArchType tyfieldTail
       ;

tyfieldTail:
           | COMMA tyfieldArchType tyfieldTail
           ;

tyfieldArchType: ID COLON typeid
               ;

vardecl: VAR ID ASSIGN exp
       | VAR ID COLON typeid ASSIGN exp
       ;

funcdecl: FUNCTION ID LPAREN tyfield RPAREN EQ exp
        | FUNCTION ID LPAREN tyfield RPAREN COLON typeid EQ exp
        ;

exp: LET decls IN optionalexplistsemicolon END 
   | arrayLHS firstRHSs
   | dotLHS rhss
   | ID
   | ID ASSIGN exp
   | MINUS exp
   | STRING
   | INT
   | NIL
   | ID LPAREN optionalexplistcomma RPAREN
   | ID LBRACE tyfieldInit RBRACE
   | LPAREN optionalexplistsemicolon RPAREN
   | exp op exp
   | IF exp THEN exp ELSE exp
   | IF exp THEN exp
   | WHILE exp DO exp 
   | FOR ID ASSIGN exp TO exp DO exp
   | BREAK
   ; 

dotLHS: ID DOT ID
      ;

arrayLHS: ID arrayBracks
        ;

arrayBracks: LBRACK exp RBRACK

firstRHSs:
    | DOT ID rhss
    | arrayBracks rhss
    | OF exp
    | ASSIGN exp
    ; 

rhss:
    | DOT ID rhss
    | arrayBracks rhss
    | ASSIGN exp
    ;

tyfieldInit:
           | tyfieldInitArchType tyfieldInitTail
           ;

tyfieldInitTail: 
               | COMMA tyfieldInitArchType tyfieldInitTail

tyfieldInitArchType: ID EQ exp
                   ;

op: PLUS
  | MINUS
  | TIMES
  | DIVIDE
  | NEQ
  | EQ
  | LT
  | LE
  | GT
  | GE
  | AND
  | OR
  | LT GT 
  ; 


optionalexplistsemicolon:
                        | explistsemicolon
                        ;

explistsemicolon: exp explisttailsemicolon
                ; 

explisttailsemicolon:
                    | SEMICOLON exp explisttailsemicolon
                    ;


optionalexplistcomma:
                    | explistcomma
                    ;
explistcomma: exp explisttailcomma
            ; 
explisttailcomma:
                | COMMA exp explisttailcomma
                ;


%%
