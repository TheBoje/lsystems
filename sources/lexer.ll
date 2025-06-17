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
#define SET_CURRENT_TOKEN(t) current_token=strdup(yytext); current_token_type=strdup(TOSTRING(t));
#define RESET_CURRENT current_token=nullptr;current_token_type=nullptr;

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

SYMBOL  [[:alnum:]\[\]\&\^\\\/\|\+\-]+

%%

"DERIVATION:"           { SET_CURRENT_TOKEN(DERIVATION);      return DERIVATION; }
"ANGLE_FACTOR:"         { SET_CURRENT_TOKEN(ANGLE_FACTOR);    return ANGLE_FACTOR; }
"SCALE_FACTOR:"         { SET_CURRENT_TOKEN(SCALE_FACTOR);    return SCALE_FACTOR; }
"AXIOM:"                { SET_CURRENT_TOKEN(AXIOM);           return AXIOM; }
"IGNORE:"               { SET_CURRENT_TOKEN(IGNORE);          return IGNORE; }
"PRODUCTIONS:"          { SET_CURRENT_TOKEN(PRODUCTIONS);     return PRODUCTIONS; }
"END_PRODUCTIONS"       { SET_CURRENT_TOKEN(END_PRODUCTIONS); return END_PRODUCTIONS; }

"-->"                   { SET_CURRENT_TOKEN(IMPLIES);         return IMPLIES; }
"<"                     { SET_CURRENT_TOKEN(CONTEXT_LEFT);    return CONTEXT_LEFT; }
">"                     { SET_CURRENT_TOKEN(CONTEXT_RIGHT);   return CONTEXT_RIGHT; }
";"                     { SET_CURRENT_TOKEN(SEMICOLON);       return SEMICOLON; }
"*"                     { SET_CURRENT_TOKEN(ANY_TOKEN);       return ANY_TOKEN; }

[0-9]+                  { SET_CURRENT_TOKEN(INTEGER); yylval.ival = atoi(yytext); return INTEGER; }
[0-9]+[.][0-9]+         { SET_CURRENT_TOKEN(FLOAT); yylval.fval = atof(yytext); return FLOAT; }
{SYMBOL}                { SET_CURRENT_TOKEN(SYMBOL); yylval.sval = strdup(yytext); return SYMBOL; }

[ \t\r\n]*              { RESET_CURRENT; /* whitespace */ }
"#".*                   { RESET_CURRENT; /* comments */ }
.                       { RESET_CURRENT; /* invalid tokens */ }

%%

int yywrap() {
    return 1;
}
