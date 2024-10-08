%{
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "ast/ast.h"
#include "ast/configuration.h"

std::vector<ast::node*> vAst;
ast::configuration* config = new ast::configuration();

#include "locations.h"

// #define LSY_DEBUG
#ifdef LSY_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

extern "C" int yylex(void);
extern "C" void yyerror(const char* message);

extern int yylineno;
extern YYLTYPE yylloc;
extern char* current_token;
extern char* current_token_type;
%}

%code requires {
    #include "ast/ast.h"
    #include "ast/configuration.h"
}

%locations
%start input

%token DERIVATION ANGLE_FACTOR SCALE_FACTOR AXIOM IGNORE PRODUCTIONS END_PRODUCTIONS
%token CONTEXT_LEFT CONTEXT_RIGHT IMPLIES SEMICOLON
%token FLOAT INTEGER ANY_TOKEN

%token <symbol_node> SYMBOL
%type  <context_node> context
%type  <symbol_node> symbol
%type  <production_node> production

%union {
    int ival;
    float fval;
    char* sval;

    ast::context_node *context_node;
    ast::symbol_node *symbol_node;
    ast::replacement_node *replacement_node;
    ast::production_node *production_node;
}

%%

input:
    | config axiom ignore productions {}
    ;

config:
    | derivation angle_factor scale_factor {}
    ;

derivation:
    | DERIVATION INTEGER SEMICOLON {
        DEBUG_PRINT("Derivation set to: %d\n", yylval.ival);
        config->derivation = yylval.ival;
    }
    ;

angle_factor:
    | ANGLE_FACTOR FLOAT SEMICOLON {
        DEBUG_PRINT("Angle factor set to: %f\n", yylval.fval);
        config->angle_factor = yylval.fval;
    }
    ;

scale_factor:
    | SCALE_FACTOR FLOAT SEMICOLON {
        DEBUG_PRINT("Scale factor set to: %f\n", yylval.fval);
        config->scale_factor = yylval.fval;
    }
    ;

axiom:
    | AXIOM SYMBOL SEMICOLON {
        DEBUG_PRINT("Axiom set to: %s\n", yylval.sval);
        config->axiom = yylval.sval;
    }
    ;

ignore:
    | IGNORE SYMBOL SEMICOLON {
        DEBUG_PRINT("Ignore set to: %s\n", yylval.sval);
        config->ignore = yylval.sval;
    }
    ;

productions:
    | PRODUCTIONS production_list END_PRODUCTIONS { DEBUG_PRINT("Productions parsed.\n"); }
    ;

production_list:
    | production production_list { DEBUG_PRINT("Production list extended.\n"); }
    | production { DEBUG_PRINT("Single production added.\n"); }
    ;

production:
    | context CONTEXT_LEFT symbol CONTEXT_RIGHT context IMPLIES SYMBOL SEMICOLON {
        auto* replacement = new ast::replacement_node(yylval.sval);
        DEBUG_PRINT("Production created with left context: %s, symbol: %s, right context: %s, replacement: %s\n",
            ($1 ? $1->context_symbols.c_str() : "null"),
            $3->symbol.c_str(),
            ($5 ? $5->context_symbols.c_str() : "null"),
            replacement->result_symbol.c_str());
        vAst.push_back(new ast::production_node($1, $3, $5, replacement));
    }
    ;

context:
    | ANY_TOKEN { DEBUG_PRINT("Context set to null.\n"); $$ = nullptr; }
    | SYMBOL { DEBUG_PRINT("Context set to: %s\n", yylval.sval); $$ = new ast::context_node(yylval.sval); }
    ;

symbol:
    | SYMBOL {
        DEBUG_PRINT("Symbol recognized: %s\n", yylval.sval);
        $$ = new ast::symbol_node(yylval.sval);
    }
    ;

%%

void yyerror(const char *message) {
    printf("Error \"%s\": line %d, column %d to line %d, column %d\n",
           message,
           yylloc.first_line,
           yylloc.first_column,
           yylloc.last_line,
           yylloc.last_column);

    if (current_token) {
        printf("Failed at token: %s (%s)\n", current_token, current_token_type);
    }
}
