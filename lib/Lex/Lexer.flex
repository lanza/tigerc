%option yylineno c++ noyywrap
%{
#include <string.h>

#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>
#include <tigerc/Parse/Parser.h>

int charPos=1;

YYSTYPE yylval;

#define ADJUST EM_tokPos = charPos; charPos += YYLeng

%}

%x INCOMMENT
%x INSTRING

%%

"/*"                               { ADJUST(); BEGIN(INCOMMENT); }
<INCOMMENT>"*/"                    { ADJUST(); BEGIN(INITIAL); }
<INCOMMENT>([^*]|\n)+|.              /* none */
<INCOMMENT><<EOF>>                 { ADJUST(); EM_error(EM_tokPos, "unterminated comment"); return EOF; }

\"                                 { ADJUST(); BEGIN(INSTRING); }
<INSTRING>\"                       { ADJUST(); BEGIN(INITIAL); yylval.sval = String(yytext); return STRING; }
<INSTRING>[a-zA-Z]
<INSTRING><<EOF>>                  { ADJUST(); EM_error(EM_tokPos, "unterminated string literal"); return EOF; } 

" "                                { ADJUST(); continue; }
\t                                 { ADJUST(); continue; }
\n                                 { ADJUST(); EM_newline(); continue; }


[0-9]+                             { ADJUST(); yylval.ival = atoi(yytext); return INT; }


":="                               { ADJUST(); return ASSIGN; }
","                                { ADJUST(); return COMMA; }
=                                  { ADJUST(); return EQ; }
:                                  { ADJUST(); return COLON; }
"["                                { ADJUST(); return LBRACK; }
"]"                                { ADJUST(); return RBRACK; }
"("                                { ADJUST(); return LPAREN; }
")"                                { ADJUST(); return RPAREN; }
"{"                                { ADJUST(); return LBRACE; }
"}"                                { ADJUST(); return RBRACE; }
"."                                { ADJUST(); return DOT; }
";"                                { ADJUST(); return SEMICOLON; }

"+"                                { ADJUST(); return PLUS; } 
"-"                                { ADJUST(); return MINUS; }
"*"                                { ADJUST(); return TIMES; }
"/"                                { ADJUST(); return DIVIDE; }
"!="                               { ADJUST(); return NEQ; }
"<"                                { ADJUST(); return LT; }
"<="                               { ADJUST(); return LE; }
">"                                { ADJUST(); return GT; }
">="                               { ADJUST(); return GE; }


and                                { ADJUST(); return AND; }
\&                                 { ADJUST(); return AND; }
or                                 { ADJUST(); return OR; }
\|                                 { ADJUST(); return OR; }
assign                             { ADJUST(); return ASSIGN; }
array                              { ADJUST(); return ARRAY; }
if                                 { ADJUST(); return IF; }
then                               { ADJUST(); return THEN; }
else                               { ADJUST(); return ELSE; }
while                              { ADJUST(); return WHILE; }
for                                { ADJUST(); return FOR; }
to                                 { ADJUST(); return TO; } 
do                                 { ADJUST(); return DO; }
let                                { ADJUST(); return LET; } 
in                                 { ADJUST(); return IN; }
end                                { ADJUST(); return END; }
of                                 { ADJUST(); return OF; }
break                              { ADJUST(); return BREAK; }
nil                                { ADJUST(); return NIL; }
function                           { ADJUST(); return FUNCTION; }
var                                { ADJUST(); return VAR; }
type                               { ADJUST(); return TYPE; }

[a-zA-Z][a-zA-Z0-9_]*              { ADJUST(); yylval.sval = String(yytext); return ID; }

 /* else */
.                                  { ADJUST(); EM_error(EM_tokPos, "illegal token"); return ILLEGAL; }
