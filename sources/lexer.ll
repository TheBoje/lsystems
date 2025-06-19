%{
extern "C" int yylex();
extern int yylineno;

#include "locations.h"
extern YYLTYPE yylloc;
#include "parser.tab.h"

#include <string.h>
#include <stdlib.h>

char* current_token = nullptr;
char* current_token_type = nullptr;

#define TOSTRING(x) #x
#define SET_CURRENT_TOKEN(t)                \
    current_token=strdup(yytext);           \
    current_token_type=strdup(TOSTRING(t));
#define RESET_CURRENT                       \
    current_token=nullptr;                  \
    current_token_type=nullptr;

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

LETTER  [A-Za-z]
DIGIT   [0-9]
INT     {DIGIT}+
FLOAT   {DIGIT}+"."{DIGIT}+

%%

"AXIOM"                { SET_CURRENT_TOKEN(AXIOM);           return AXIOM; }
"IGNORE"               { SET_CURRENT_TOKEN(IGNORE);          return IGNORE; }
"PRODUCTIONS"          { SET_CURRENT_TOKEN(PRODUCTIONS);     return PRODUCTIONS; }
"END_PRODUCTIONS"       { SET_CURRENT_TOKEN(END_PRODUCTIONS); return END_PRODUCTIONS; }
"DEFINITIONS"          { SET_CURRENT_TOKEN(DEFINTIONS);      return DEFINITIONS; }
"END_DEFINITIONS"       { SET_CURRENT_TOKEN(END_DEFINITIONS); return END_DEFINITIONS; }

"-->"                   { SET_CURRENT_TOKEN(IMPLIES);         return IMPLIES; }
"<"                     { SET_CURRENT_TOKEN(OP_LT);           return OP_LT; }
"<="                    { SET_CURRENT_TOKEN(OP_LTE);          return OP_LTE; }
">"                     { SET_CURRENT_TOKEN(OP_GT);           return OP_GT; }
">="                    { SET_CURRENT_TOKEN(OP_GTE);          return OP_GTE; }
"=="                    { SET_CURRENT_TOKEN(OP_EQ);           return OP_EQ; }
"!="                    { SET_CURRENT_TOKEN(OP_NEQ);          return OP_NEQ; }
";"                     { SET_CURRENT_TOKEN(SEMICOLON);       return SEMICOLON; }
":"                     { SET_CURRENT_TOKEN(COLON);           return COLON; }
","                     { SET_CURRENT_TOKEN(COMMA);           return COMMA; }
"+"                     { SET_CURRENT_TOKEN(OP_PLUS);         return OP_PLUS; }
"-"                     { SET_CURRENT_TOKEN(OP_MINUS);        return OP_MINUS; }
"*"                     { SET_CURRENT_TOKEN(OP_MULT);         return OP_MULT; }
"/"                     { SET_CURRENT_TOKEN(OP_DIVIDE);       return OP_DIVIDE; }
"^"                     { SET_CURRENT_TOKEN(OP_EXPONENT);     return OP_EXPONENT; }
"!"                     { SET_CURRENT_TOKEN(OP_NOT);          return OP_NOT; }
"&"                     { SET_CURRENT_TOKEN(OP_AND);          return OP_AND; }
"|"                     { SET_CURRENT_TOKEN(OP_OR);           return OP_OR; }
"("                     { SET_CURRENT_TOKEN(PAR_OPEN);        return PAR_OPEN; }
")"                     { SET_CURRENT_TOKEN(PAR_CLOSE);       return PAR_CLOSE; }

{INT}                   { SET_CURRENT_TOKEN(INTEGER); yylval.ival = atoi(yytext);  return INTEGER; }
{FLOAT}                 { SET_CURRENT_TOKEN(FLOAT); yylval.fval = atof(yytext);    return FLOAT; }
{LETTER}({LETTER}|{DIGIT})* { SET_CURRENT_TOKEN(IDENTIFIER); yylval.sval = strdup(yytext); return IDENTIFIER; }
[ \t\r\n]*              { RESET_CURRENT; /* whitespace */ }
"#".*                   { RESET_CURRENT; /* comments */ }
.                       { RESET_CURRENT; /* invalid tokens */ }

%%

int yywrap() {
    return 1;
}
