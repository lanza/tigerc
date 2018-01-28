%skeleton "lalr1.cc"
%require "3.0"
%defines 
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { tlang }

%code requires
{
  #include <iostream>
  #include <string>
  #include <vector>
  #include <stdint.h>

  namespace tlang {
    class Lexer;
    class Interpreter;
  }
}
%code top
{
  #include <iostream>
  #include <tigerc/Parse/Parser.h>
  #include <tigerc/Lex/Lexer.h>

  static tlang::Parser::symbol_type yylex(tlang::Lexer &lexer, tlang::Interpreter &driver) {
    return lexer.get_next_token();
  }

  //maybe define a macro that will do this? 
  // #define yylex(x, y) scanner.get_next_token()

  using namespace tlang;
}

%lex-param { tlang::Lexer &lexer }
%lex-param { tlang::Interpreter &driver }
%parse-param { tlang::Lexer &lexer }
%parse-param { tlang::Interpreter &driver }
%locations
/*%define parse.trace      used as ${debug_flag} in cmake */
%define parse.error verbose

%define api.token.prefix {TOKEN_}

/*%type< tlang::Command > command; */
/*%type< std::vector<uint64_t> > arguments; */

%right ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE 
%left PLUS MINUS
%left TIMES DIVIDE

%token EOF 0 "end of file"
%token <std::string> ID STRING "string";
%token <int> INT "int"; 
%token LPAREN "leftparen";
%token RPAREN "rightparen";
%token COMMA "comma";
%token COLON "colon"; 
%token SEMICOLON "semicolon";
%token LBRACK "lbrack";
%token RBRACK "rbrack";
%token LBRACE "lbrace";
%token RBRACE "rbrace";
%token DOT "dot";
%token PLUS "plus";
%token MINUS "minus";
%token TIMES "times";
%token DIVIDE "divide";
%token EQ "eq";
%token NEQ "neq";
%token LT "lt";
%token LE "le";
%token GT "gt";
%token GE "ge";
%token AND "and";
%token OR "or";
%token ASSIGN "assign";
%token ARRAY "array";
%token IF "if";
%token THEN "then";
%token ELSE "else";
%token WHILE "while";
%token FOR "for";
%token TO "to";
%token DO "do";
%token LET "let";
%token IN "in";
%token END "end";
%token OF "of";
%token BREAK "break";
%token NIL "nil";
%token FUNCTION "function";
%token VAR "var";
%token TYPE "type";
%token ILLEGAL "illegal";

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

void tlang::Parser::error(const location &loc, const std::string &message) {
  //TODO figure out what to do with the NYI location - source says driver.location()
  std::cout << "Error: " << message << std::endl << "Error location: " << "NYI" << std::endl;
}
