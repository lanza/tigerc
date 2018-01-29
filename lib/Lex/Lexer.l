%{

#include <iostream>
#include <cstdlib>

#include <tigerc/Interpreter/Interpreter.h>
#include <tigerc/Lex/Lexer.h>
#include <tigerc/Parse/Parser.h>

#include <tigerc/Parse/location.hh> 

#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>

int charPos=1;

#define ADJUST EM_tokPos = charPos; charPos += YYLeng

#define yyterminate() tlang::Parser::make_ENDOFFILE(tlang::location());
#define YY_USER_ACTION ;
#define YY_NO_UNISTD_H

%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Lexer"
%option prefix="tlang_"
%option debug

%x INCOMMENT
%x INSTRING

%%

"/*"                               { ADJUST(); BEGIN(INCOMMENT); }
<INCOMMENT>"*/"                    { ADJUST(); BEGIN(INITIAL); }
<INCOMMENT>([^*]|\n)+|.              /* none */
<INCOMMENT><<EOF>>                 { ADJUST(); EM_error(EM_tokPos, "unterminated comment"); 
                                     return tlang::Parser::make_ENDOFFILE(tlang::location()); 
                                   }

\"                                 { ADJUST(); BEGIN(INSTRING); }
<INSTRING>\"                       { ADJUST(); BEGIN(INITIAL); 
                                     return tlang::Parser::make_STRING(yytext, tlang::location()); 
                                   } 
<INSTRING>[a-zA-Z]
<INSTRING>" "
<INSTRING><<EOF>>                  { ADJUST(); EM_error(EM_tokPos, "unterminated string literal"); 
                                     tlang::Parser::make_ENDOFFILE(tlang::location());
                                   }

" "                                { ADJUST(); continue; }
\t                                 { ADJUST(); continue; }
\n                                 { ADJUST(); EM_newline(); continue; }


[0-9]+                             { ADJUST(); 
                                     int number = strtoull(yytext, 0, 10);
                                     return tlang::Parser::make_INT(number, tlang::location());
                                   }


":="                               { ADJUST(); return tlang::Parser::make_ASSIGN(tlang::location()); }
","                                { ADJUST(); return tlang::Parser::make_COMMA(tlang::location()); }
=                                  { ADJUST(); return tlang::Parser::make_EQ(tlang::location()); }
:                                  { ADJUST(); return tlang::Parser::make_COLON(tlang::location()); }
"["                                { ADJUST(); return tlang::Parser::make_LBRACK(tlang::location()); }
"]"                                { ADJUST(); return tlang::Parser::make_RBRACK(tlang::location()); }
"("                                { ADJUST(); return tlang::Parser::make_LPAREN(tlang::location()); }
")"                                { ADJUST(); return tlang::Parser::make_RPAREN(tlang::location()); }
"{"                                { ADJUST(); return tlang::Parser::make_LBRACE(tlang::location()); }
"}"                                { ADJUST(); return tlang::Parser::make_RBRACE(tlang::location()); }
"."                                { ADJUST(); return tlang::Parser::make_DOT(tlang::location()); }
";"                                { ADJUST(); return tlang::Parser::make_SEMICOLON(tlang::location()); }

"+"                                { ADJUST(); return tlang::Parser::make_PLUS(tlang::location()); } 
"-"                                { ADJUST(); return tlang::Parser::make_MINUS(tlang::location()); }
"*"                                { ADJUST(); return tlang::Parser::make_TIMES(tlang::location()); }
"/"                                { ADJUST(); return tlang::Parser::make_DIVIDE(tlang::location()); }
"!="                               { ADJUST(); return tlang::Parser::make_NEQ(tlang::location()); }
"<"                                { ADJUST(); return tlang::Parser::make_LT(tlang::location()); }
"<="                               { ADJUST(); return tlang::Parser::make_LE(tlang::location()); }
">"                                { ADJUST(); return tlang::Parser::make_GT(tlang::location()); }
">="                               { ADJUST(); return tlang::Parser::make_GE(tlang::location()); }


and                                { ADJUST(); return tlang::Parser::make_AND(tlang::location()); }
\&                                 { ADJUST(); return tlang::Parser::make_AND(tlang::location()); }
or                                 { ADJUST(); return tlang::Parser::make_OR(tlang::location()); }
\|                                 { ADJUST(); return tlang::Parser::make_OR(tlang::location()); }
assign                             { ADJUST(); return tlang::Parser::make_ASSIGN(tlang::location()); }
array                              { ADJUST(); return tlang::Parser::make_ARRAY(tlang::location()); }
if                                 { ADJUST(); return tlang::Parser::make_IF(tlang::location()); }
then                               { ADJUST(); return tlang::Parser::make_THEN(tlang::location()); }
else                               { ADJUST(); return tlang::Parser::make_ELSE(tlang::location()); }
while                              { ADJUST(); return tlang::Parser::make_WHILE(tlang::location()); }
for                                { ADJUST(); return tlang::Parser::make_FOR(tlang::location()); }
to                                 { ADJUST(); return tlang::Parser::make_TO(tlang::location()); } 
do                                 { ADJUST(); return tlang::Parser::make_DO(tlang::location()); }
let                                { ADJUST(); return tlang::Parser::make_LET(tlang::location()); } 
in                                 { ADJUST(); return tlang::Parser::make_IN(tlang::location()); }
end                                { ADJUST(); return tlang::Parser::make_END(tlang::location()); }
of                                 { ADJUST(); return tlang::Parser::make_OF(tlang::location()); }
break                              { ADJUST(); return tlang::Parser::make_BREAK(tlang::location()); }
nil                                { ADJUST(); return tlang::Parser::make_NIL(tlang::location()); }
function                           { ADJUST(); return tlang::Parser::make_FUNCTION(tlang::location()); }
var                                { ADJUST(); return tlang::Parser::make_VAR(tlang::location()); }
type                               { ADJUST(); return tlang::Parser::make_TYPE(tlang::location()); }

[a-zA-Z][a-zA-Z0-9_]*              { ADJUST(); 
                                     return tlang::Parser::make_ID(yytext, tlang::location());
                                   }
<<EOF>>                            { ADJUST(); return tlang::Parser::make_ENDOFFILE(tlang::location()); }

 /* else */
.                                  { ADJUST(); EM_error(EM_tokPos, "illegal token"); 
                                     return tlang::Parser::make_ILLEGAL(tlang::location()); 
                                   }
