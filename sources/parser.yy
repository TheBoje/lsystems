%{
#include <stdio.h>
#include <stdlib.h>

#include "locations.h"

#include "ast/node.h"
#include "ast/program.h"
#include "ast/definition.h"
#include "ast/symbol.h"

#define LSY_DEBUG
#ifdef LSY_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

// ast::Node* root;

extern "C" int yylex(void);
extern "C" void yyerror(ast::program ** res, const char* message);
extern int yylineno;

extern YYLTYPE yylloc;
%}

%locations
%start program

%token INTEGER FLOAT
%token CHARACTER NUMBER PLUS MINUS STAR FORWARD_SLASH EQUALS SEMICOLON LPAREN RPAREN
%token LCONTEXT RCONTEXT ARROW LBRACKET RBRACKET COLON
%token DERIVATION ANGLE_FACTOR SCALE_FACTOR AXIOM IGNORE PRODUCTIONS END_PRODUCTIONS

%type<program> program
%type<term> definition symbol

%union {
    int num_int;
    float num_float;
    char* c;

    ast::program* program;
    ast::definition* definition;
    ast::symbol* symbol;
}

%code requires {
    #include "ast/program.h"
    #include "ast/definition.h"
    #include "ast/symbol.h"
}

%parse-param { ast::program ** res }

%code provides {
    int yyparse(ast::program ** res);
}

%%

program: sequence { DEBUG_PRINT("program -> sequence\n"); *res = new ast::sequence($1); }
    ;

sequence: expression sequence { DEBUG_PRINT("sequence -> expression sequence\n"); $$ = new seq_item($1, $2); }
    | /* empty */ { DEBUG_PRINT("sequence -> nullptr\n"); $$ = nullptr; }
    ;

expression: DERIVATION COLON INTEGER SEMICOLON{
        DEBUG_PRINT("definition -> DERIVATION COLON INTEGER ; (%d)\n", yylval.num_int);
        $$ = new ast::definition(yylval.num_int, ast::definition::type::derivation);
    }
    | ANGLE_FACTOR COLON FLOAT SEMICOLON {
        DEBUG_PRINT("definition -> ANGLE_FACTOR COLON FLOAT ; (%f)\n", yylval.num_float);
        $$ = new ast::definition(yylval.num_float, ast::definition::type::angle_factor);
    }
    | SCALE_FACTOR COLON FLOAT SEMICOLON {
        DEBUG_PRINT("definition -> SCALE_FACTOR COLON FLOAT ; (%f)\n", yylval.num_float);
        $$ = new ast::definition(yylval.num_float, ast::definition::type::scale);
    }
    | AXIOM COLON symbols SEMICOLON {
        DEBUG_PRINT("definition -> AXIOM COLON symbols ;\n");
        $$ = new ast::symbol($3, ast::symbol::type::axiom);
    }
    | IGNORE COLON symbols SEMICOLON {
        DEBUG_PRINT("definition -> IGNORE COLON symbols ;\n");
        $$ = new ast::symbol($3, ast::symbol::type::ignore);
    }
    | PRODUCTIONS COLON productions END_PRODUCTIONS { DEBUG_PRINT("definition -> PRODUCTIONS productions END_PRODUCTIONS\n"); }
    ;

symbols: symbol symbols { DEBUG_PRINT("symbols -> symbol symbols\n"); }
    | /* empty */ { DEBUG_PRINT("symbols -> /* empty */\n"); }
    ;

symbol: CHARACTER { DEBUG_PRINT("symbol -> CHARACTER (%s)\n", yylval.c); }
      | NUMBER { DEBUG_PRINT("symbol -> NUMBER (%d)\n", yylval.num_int); }
      | LBRACKET { DEBUG_PRINT("symbol -> LBRACKET ([)\n"); }
      | RBRACKET { DEBUG_PRINT("symbol -> RBRACKET (])\n"); }
      | PLUS { DEBUG_PRINT("symbol -> PLUS (+)\n"); }
      | MINUS { DEBUG_PRINT("symbol -> MINUS (-)\n"); }
      | STAR { DEBUG_PRINT("symbol -> STAR (*)\n"); }
      | FORWARD_SLASH { DEBUG_PRINT("symbol -> FORWARD_SLASH (/)\n"); }
      ;

productions: production productions { DEBUG_PRINT("productions -> production productions\n"); }
    | production { DEBUG_PRINT("productions -> production\n"); }
    ;

production: symbols LCONTEXT symbols RCONTEXT symbols ARROW symbols SEMICOLON { DEBUG_PRINT("production -> symbols LCONTEXT symbols RCONTEXT symbols ARROW symbols :\n"); }
    ;

%%

void yyerror(ast::program ** res, const char *message) {
    (void)res;

    fprintf(stderr, "Error at line %d, column %d: %s\n",
                    yylloc.last_line,
                    yylloc.last_column,
                    message);

    printf("Location: line %d, column %d to line %d, column %d\n",
           yylloc.first_line,
           yylloc.first_column,
           yylloc.last_line,
           yylloc.last_column);
}
