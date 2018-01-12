%option yylineno
%{
#include <string.h>

#include <tigerc/Support/errormsg.h>
#include <tigerc/Support/util.h>
#include <parse.h>

int charPos=1;

int yywrap(void) {
 charPos = 1;
 return 1;
} 

YYSTYPE yylval;

void adjust(void) {
 EM_tokPos = charPos;
 charPos += yyleng;
}

%}

%x INCOMMENT
%x INSTRING

%%

"/*"                               { adjust(); BEGIN(INCOMMENT); }
<INCOMMENT>"*/"                    { adjust(); BEGIN(INITIAL); }
<INCOMMENT>([^*]|\n)+|.              /* none */
<INCOMMENT><<EOF>>                 { adjust(); EM_error(EM_tokPos, "unterminated comment"); return EOF; }

\"                                 { adjust(); BEGIN(INSTRING); }
<INSTRING>\"                       { adjust(); BEGIN(INITIAL); yylval.sval = String(yytext); return STRING; }
<INSTRING>[a-zA-Z]
<INSTRING><<EOF>>                  { adjust(); EM_error(EM_tokPos, "unterminated string literal"); return EOF; } 

" "                                { adjust(); continue; }
\t                                 { adjust(); continue; }
\n                                 { adjust(); EM_newline(); continue; }


[0-9]+                             { adjust(); yylval.ival = atoi(yytext); return INT; }


":="                               { adjust(); return ASSIGN; }
","                                { adjust(); return COMMA; }
=                                  { adjust(); return EQ; }
:                                  { adjust(); return COLON; }
"["                                { adjust(); return LBRACK; }
"]"                                { adjust(); return RBRACK; }
"("                                { adjust(); return LPAREN; }
")"                                { adjust(); return RPAREN; }
"{"                                { adjust(); return LBRACE; }
"}"                                { adjust(); return RBRACE; }
"."                                { adjust(); return DOT; }
";"                                { adjust(); return SEMICOLON; }

"+"                                { adjust(); return PLUS; } 
"-"                                { adjust(); return MINUS; }
"*"                                { adjust(); return TIMES; }
"/"                                { adjust(); return DIVIDE; }
"!="                               { adjust(); return NEQ; }
"<"                                { adjust(); return LT; }
"<="                               { adjust(); return LE; }
">"                                { adjust(); return GT; }
">="                               { adjust(); return GE; }


and                                { adjust(); return AND; }
or                                 { adjust(); return OR; }
assign                             { adjust(); return ASSIGN; }
array                              { adjust(); return ARRAY; }
if                                 { adjust(); return IF; }
then                               { adjust(); return THEN; }
else                               { adjust(); return ELSE; }
while                              { adjust(); return WHILE; }
for                                { adjust(); return FOR; }
to                                 { adjust(); return TO; } 
do                                 { adjust(); return DO; }
let                                { adjust(); return LET; } 
in                                 { adjust(); return IN; }
end                                { adjust(); return END; }
of                                 { adjust(); return OF; }
break                              { adjust(); return BREAK; }
nil                                { adjust(); return NIL; }
function                           { adjust(); return FUNCTION; }
var                                { adjust(); return VAR; }
type                               { adjust(); return TYPE; }

[a-zA-Z][a-zA-Z0-9_]*              { adjust(); yylval.sval = String(yytext); return ID; }

 /* else */
.                                  { adjust(); EM_error(EM_tokPos, "illegal token"); return ILLEGAL; }
