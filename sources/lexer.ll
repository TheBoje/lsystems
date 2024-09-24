%{
extern "C" int yylex();
extern int yylineno;

#include "locations.h"
extern YYLTYPE yylloc;
#include "parser.tab.h"

typedef int offset_t;
offset_t offset = 0;

#define YY_USER_ACTION         \
  yylloc.first_line = yylloc.last_line = yylineno; \
  yylloc.first_column = offset + 1;                \
  yylloc.last_column = offset + yyleng;            \
  offset += yyleng;
%}

%option yylineno
%option noinput nounput

%%

DERIVATION        { return DERIVATION; }
ANGLE_FACTOR      { return ANGLE_FACTOR; }
SCALE_FACTOR      { return SCALE_FACTOR; }
AXIOM             { return AXIOM; }
IGNORE            { return IGNORE; }
PRODUCTIONS       { return PRODUCTIONS; }
END_PRODUCTIONS   { return END_PRODUCTIONS; }

[A-Za-z]   { yylval.c = strdup(yytext); return CHARACTER; }
[0-9]      { yylval.num_int = atoi(yytext); return NUMBER; }
[0-9]+     { yylval.num_int = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+    { yylval.num_float = atof(yytext); return FLOAT; }

###(.|\n)*###   { }
#.*"\n"         { }

"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return STAR; }
"/"        { return FORWARD_SLASH; }
"="        { return EQUALS; }
":"        { return COLON; }
";"        { return SEMICOLON; }
"("        { return LPAREN; }
")"        { return RPAREN; }
"["        { return LBRACKET; }
"]"        { return RBRACKET; }
"-->"      { return ARROW; }
">"        { return RCONTEXT; }
"<"        { return LCONTEXT; }
" "        { /* ignore whitespace */ }
\n         { offset = 0; /* reset offset at newline */ }

.          { /* ignore any other character */ }

%%

int yywrap() {
    return 1;
}
