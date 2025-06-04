%{
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string.h>

#include "ast/ast.h"
#include "ast/configuration.h"

std::vector<ast::node*> vAst;
ast::configuration* config = new ast::configuration();

#include "locations.h"

// #define LSY_DEBUG
#ifdef LSY_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) (void)nullptr;
#endif

extern "C" int yylex(void);
extern "C" void yyerror(const char* message);

extern int yylineno;
extern YYLTYPE yylloc;
extern char* current_file_path;
extern char* current_token;
extern char* current_token_type;

bool has_printed_line_info = false;
%}

%code requires {
    #include "ast/ast.h"
    #include "ast/configuration.h"
}

%locations
%define parse.error detailed
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

input: config axiom ignore productions {} ;

config: derivation angle_factor scale_factor {} ;

derivation: DERIVATION INTEGER SEMICOLON { DEBUG_PRINT("Derivation set to: %d\n", yylval.ival); config->derivation = yylval.ival; }
    | DERIVATION INTEGER error { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    ;

angle_factor: ANGLE_FACTOR FLOAT SEMICOLON { DEBUG_PRINT("Angle factor set to: %f\n", yylval.fval); config->angle_factor = yylval.fval; }
    | ANGLE_FACTOR FLOAT error { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    ;

scale_factor: SCALE_FACTOR FLOAT SEMICOLON { DEBUG_PRINT("Scale factor set to: %f\n", yylval.fval); config->scale_factor = yylval.fval; }
    | SCALE_FACTOR FLOAT error { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    ;

axiom: AXIOM SYMBOL SEMICOLON { DEBUG_PRINT("Axiom set to: %s\n", yylval.sval); config->axiom = yylval.sval; }
    | AXIOM SYMBOL error { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    ;

ignore: IGNORE SYMBOL SEMICOLON { DEBUG_PRINT("Ignore set to: %s\n", yylval.sval); config->ignore = yylval.sval; }
    | IGNORE SYMBOL error { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    ;

productions: PRODUCTIONS production_list END_PRODUCTIONS SEMICOLON { DEBUG_PRINT("Productions parsed.\n"); }
    | PRODUCTIONS production_list END_PRODUCTIONS error     { yyerror("syntax error: expected semicolon");                 return EXIT_FAILURE; }
    | PRODUCTIONS production_list error           SEMICOLON { yyerror("syntax error: expected 'END_PRODUCTIONS' keyword"); return EXIT_FAILURE; }
    | error       production_list END_PRODUCTIONS SEMICOLON { yyerror("syntax error: expected 'PRODUCTIONS' keyword");     return EXIT_FAILURE; }
    ;

production_list: production { DEBUG_PRINT("Single production added.\n"); }
    | production production_list { DEBUG_PRINT("Production list extended.\n"); }
    ;

production: context CONTEXT_LEFT symbol CONTEXT_RIGHT context IMPLIES symbol SEMICOLON {
        auto* replacement = new ast::replacement_node($7);
        DEBUG_PRINT("Production created with left context: %s, symbol: %s, right context: %s, replacement: %s\n",
            ($1 ? $1->context_symbols.c_str() : "null"),
            $3->symbol.c_str(),
            ($5 ? $5->context_symbols.c_str() : "null"),
            replacement->result_symbol->symbol.c_str());
        vAst.push_back(new ast::production_node($1, $3, $5, replacement));
    }
    | context CONTEXT_LEFT symbol CONTEXT_RIGHT context IMPLIES symbol error     { yyerror("syntax error: expected semicolon"); return EXIT_FAILURE; }
    | context CONTEXT_LEFT symbol CONTEXT_RIGHT context error   symbol SEMICOLON { yyerror("syntax error: expected '-->'");     return EXIT_FAILURE; }
    | context CONTEXT_LEFT symbol error         context IMPLIES symbol SEMICOLON { yyerror("syntax error: expected '>'");       return EXIT_FAILURE; }
    | context error        symbol CONTEXT_RIGHT context IMPLIES symbol SEMICOLON { yyerror("syntax error: expected '<'");       return EXIT_FAILURE; }
    ;

context: ANY_TOKEN { DEBUG_PRINT("Context set to null.\n"); $$ = nullptr; }
    | SYMBOL { DEBUG_PRINT("Context set to: %s\n", yylval.sval); $$ = new ast::context_node(yylval.sval); }
    | INTEGER { DEBUG_PRINT("Context set to: %d\n", yylval.ival); $$ = new ast::context_node(std::to_string(yylval.ival)); }
    ;

symbol: SYMBOL { DEBUG_PRINT("Symbol recognized: %s\n", yylval.sval); $$ = new ast::symbol_node(yylval.sval); }
    | INTEGER { DEBUG_PRINT("Symbol recognized: %d\n", yylval.ival); $$ = new ast::symbol_node(std::to_string(yylval.ival)); }
    ;

%%

void yyerror(const char *message) {
    // FIXME(Louis): There is both C and C++ code here, might be easier to use only C++?

    std::ostringstream error_message;
    if (current_file_path) {
        FILE* file = fopen(current_file_path, "r");
        if (file) {
            char line[1024]; // FIXME(Louis): This could be better.
            int current_line = 1;

            while (fgets(line, sizeof(line), file)) {
                if (current_line == yylloc.first_line) {
                    error_message << line;

                    // Underline error
                    for (int i = 0; i < yylloc.first_column - 1; i++) {
                        error_message << " ";
                    }
                    for (int i = yylloc.first_column; i <= yylloc.last_column; i++) {
                        error_message << "^";
                    }
                    error_message << "\n";
                    break;
                }
                yylloc.first_column -= strlen(line);
                yylloc.last_column -= strlen(line);
                current_line++;
            }
            fclose(file);
        }
    }

    if (!has_printed_line_info) {
        printf("%s: line %d, column %d to line %d, column %d:\n",
               message,
               yylloc.first_line,
               yylloc.first_column,
               yylloc.last_line,
               yylloc.last_column);
        has_printed_line_info = true;
    }

    printf("%s\n", error_message.str().c_str());

    if (current_token) {
        printf("Failed at token: %s (%s)\n", current_token, current_token_type);
    }
}
