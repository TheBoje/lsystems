%{
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string.h>

#include "ast/ast.h"
#include "ast/configuration.h"

std::vector<ast::node*> vAst;
ast::configuration* config = new ast::configuration();

#include "locations.h"

//#define LSY_DEBUG
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
%define parse.error custom
%start input

%token DERIVATION       "DERIVATION:"
%token ANGLE_FACTOR     "ANGLE_FACTOR:"
%token SCALE_FACTOR     "SCALE_FACTOR:"
%token AXIOM            "AXIOM:"
%token IGNORE           "IGNORE:"
%token PRODUCTIONS      "PRODUCTIONS:"
%token END_PRODUCTIONS  "END_PRODUCTIONS"
%token CONTEXT_LEFT     "<"
%token CONTEXT_RIGHT    ">"
%token IMPLIES          "-->"
%token SEMICOLON        ";"
%token FLOAT            "float"
%token INTEGER          "integer"
%token ANY_TOKEN        "*"

%token <symbol_node> SYMBOL "symbol"
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
    ;

angle_factor: ANGLE_FACTOR FLOAT SEMICOLON { DEBUG_PRINT("Angle factor set to: %f\n", yylval.fval); config->angle_factor = yylval.fval; }
    ;

scale_factor: SCALE_FACTOR FLOAT SEMICOLON { DEBUG_PRINT("Scale factor set to: %f\n", yylval.fval); config->scale_factor = yylval.fval; }
    ;

axiom: AXIOM SYMBOL SEMICOLON { DEBUG_PRINT("Axiom set to: %s\n", yylval.sval); config->axiom = yylval.sval; }
    ;

ignore: IGNORE SYMBOL SEMICOLON { DEBUG_PRINT("Ignore set to: %s\n", yylval.sval); config->ignore = yylval.sval; }
    ;

productions: PRODUCTIONS production_list END_PRODUCTIONS SEMICOLON { DEBUG_PRINT("Productions parsed.\n"); }
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
    ;

context: ANY_TOKEN { DEBUG_PRINT("Context set to null.\n"); $$ = nullptr; }
    | SYMBOL { DEBUG_PRINT("Context set to: %s\n", yylval.sval); $$ = new ast::context_node(yylval.sval); }
    | INTEGER { DEBUG_PRINT("Context set to: %d\n", yylval.ival); $$ = new ast::context_node(std::to_string(yylval.ival)); }
    ;

symbol: SYMBOL { DEBUG_PRINT("Symbol recognized: %s\n", yylval.sval); $$ = new ast::symbol_node(yylval.sval); }
    | INTEGER { DEBUG_PRINT("Symbol recognized: %d\n", yylval.ival); $$ = new ast::symbol_node(std::to_string(yylval.ival)); }
    ;

%%

void yyerror(const char * /*message*/ ) {
    // FIXME(Louis): There is both C and C++ code here, might be easier to use only C++?
    // FIXME(Louis): use message? for now, only supports syntax error. Maybe report semantic errors?

    if (has_printed_line_info) {
        return;
    }

    std::ostringstream error_message;
    if (current_file_path && std::filesystem::exists(current_file_path)) {
        std::ifstream file(current_file_path);
        if (file) {
            std::string line;
            int current_line = 1;

            while (std::getline(file, line)) {
                if (current_line == yylloc.first_line) {
                    error_message << line << "\n";

                    for (int i = 0; i < yylloc.first_column - 1; i++) {
                        error_message << " ";
                    }
                    for (int i = yylloc.first_column; i <= yylloc.last_column; i++) {
                        error_message << "^";
                    }
                    error_message << "\n";
                    break;
                }

                yylloc.first_column -= static_cast<int>(line.length()) + 1;
                yylloc.last_column -= static_cast<int>(line.length()) + 1;
                current_line++;
            }
        }
    }

    printf(": line %d, column %d to line %d, column %d:\n",
        yylloc.first_line,
        yylloc.first_column,
        yylloc.last_line,
        yylloc.last_column);
    has_printed_line_info = true;

    printf("%s\n", error_message.str().c_str());
}

static int yyreport_syntax_error (const yypcontext_t *ctx)
{
    int res = 0;
    printf("syntax error");
    // Report the tokens expected at this point.
    {
        enum { TOKENMAX = 5 };
        yysymbol_kind_t expected[TOKENMAX];

        int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
        if (n < 0) {
            res = n;
        } else {
            for (int i = 0; i < n; ++i) {
                printf("%s \"%s\"",i == 0 ? ": expected" : " or", yysymbol_name (expected[i]));
            }
        }
    }

    // Report the unexpected token.
    {
        yysymbol_kind_t lookahead = yypcontext_token (ctx);
        if (lookahead != YYSYMBOL_YYEMPTY) {
            printf(" before \"%s\"", yysymbol_name (lookahead));
        }
    }
    yyerror("");
    return res;
}
