%{
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>

#include "ast/tree/axiom.h"
#include "ast/tree/comparator.h"
#include "ast/tree/condition.h"
#include "ast/tree/definition.h"
#include "ast/tree/definitions.h"
#include "ast/tree/expression.h"
#include "ast/tree/identifier.h"
#include "ast/tree/node.h"
#include "ast/tree/node_list.h"
#include "ast/tree/numerical.h"
#include "ast/tree/predecessor.h"
#include "ast/tree/production.h"
#include "ast/tree/productions.h"
#include "ast/tree/root.h"
#include "ast/tree/successor.h"
#include "ast/tree/symbol.h"

ast::node* Ast;

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
    #include "ast/tree/node.h"
    #include "ast/tree/node_list.h"

    #include <string>
    #include <vector>
}

%locations
%define parse.error custom

%token AXIOM            "AXIOM"
%token IGNORE           "IGNORE"
%token PRODUCTIONS      "PRODUCTIONS"
%token END_PRODUCTIONS  "END_PRODUCTIONS"
%token DEFINITIONS      "DEFINITIONS"
%token END_DEFINITIONS  "END_DEFINITIONS"
%token OP_LT            "<"
%token OP_LTE           "<="
%token OP_GT            ">"
%token OP_GTE           ">="
%token OP_EQ            "=="
%token OP_NEQ           "!="
%token OP_PLUS          "+"
%token OP_MINUS         "-"
%token OP_MULT          "*"
%token OP_DIVIDE        "/"
%token OP_EXPONENT      "^"
%token OP_NOT           "!"
%token OP_AND           "&"
%token OP_OR            "|"
%token PAR_OPEN         "(" 
%token PAR_CLOSE        ")"
%token IMPLIES          "-->"
%token SEMICOLON        ";"
%token COLON            ":"
%token COMMA            ","
%token <ival> FLOAT     "float"
%token <fval> INTEGER   "integer"
%token <sval> IDENTIFIER "identifier"

%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVIDE
%right OP_EXPONENT

%type <node> comparator expression condition_opt primary symbol successor predecessor production production_section definition definitions axiom input
%type <node_list> arg_list symbol_list production_list definition_list

%union {
    int ival;
    float fval;
    char* sval;

    ast::node* node;
    ast::node_list* node_list;
}

%start input

%%

input: axiom definitions production_section { $$ = new ast::root($1, $2, $3); Ast = $$; };

axiom: AXIOM COLON symbol_list SEMICOLON { $$ = new ast::axiom($3); };

definitions: /* empty */ { $$ = nullptr; }
           | DEFINITIONS COLON definition_list END_DEFINITIONS SEMICOLON { $$ = new ast::definitions($3); }

definition_list: definition { $$ = new ast::definition_list( { $1 } ); }
                | definition_list definition { $1->push_back($2); $$ = $1; }
;

definition: IDENTIFIER IMPLIES expression SEMICOLON { $$ = new ast::definition(new ast::identifier($1), $3); };

production_section: PRODUCTIONS COLON production_list END_PRODUCTIONS SEMICOLON { $$ = new ast::productions($3); };

production_list: production { $$ = new ast::production_list( { $1 } );}
               | production_list production { $1->push_back($2); $$ = $1; }
;

production: predecessor COLON condition_opt IMPLIES successor SEMICOLON { $$ = new ast::production($1, $5, $3); };

predecessor:                   symbol                   { $$ = new ast::predecessor($1); }
           |                   symbol OP_GT symbol_list { $$ = new ast::predecessor($1, nullptr, $3); }
           | symbol_list OP_LT symbol                   { $$ = new ast::predecessor($3, $1); }
           | symbol_list OP_LT symbol OP_GT symbol_list { $$ = new ast::predecessor($3, $1, $5); }
;

/* node: can only do a == b for now, maybe expand for more complex conditions */
condition_opt: /* empty */ { $$ = nullptr; }
             | expression comparator expression { $$ = new ast::condition($1, $3, $2); }
;

successor: symbol_list { $$ = new ast::successor($1); };

symbol_list: symbol { $$ = new ast::node_list( { $1 } ); }
                    | symbol_list symbol { $1->push_back($2); $$ = $1; }
;

symbol: IDENTIFIER { $$ = new ast::symbol(new ast::identifier($1)); }
               | IDENTIFIER PAR_OPEN arg_list PAR_CLOSE { $$ = new ast::symbol(new ast::identifier($1), $3); }
;

arg_list: expression { $$ = new ast::arg_list( { $1 } ); }
        | arg_list COMMA expression { $1->push_back($3); $$ = $1; }
;

/* standard arithmetic and boolean operators */
expression: expression OP_PLUS     expression { $$ = new ast::expression($1, $3, ast::operator_type::PLUS);     }
          | expression OP_MINUS    expression { $$ = new ast::expression($1, $3, ast::operator_type::MINUS);    }
          | expression OP_MULT     expression { $$ = new ast::expression($1, $3, ast::operator_type::MULT);     }
          | expression OP_DIVIDE   expression { $$ = new ast::expression($1, $3, ast::operator_type::DIVIDE);   }
          | expression OP_EXPONENT expression { $$ = new ast::expression($1, $3, ast::operator_type::EXPONENT); }
          | PAR_OPEN   expression  PAR_CLOSE  { $$ = $2; }
          | primary                           { $$ = $1; }
          ;

primary: INTEGER    { $$ = new ast::numerical<int>(yylval.ival);   }
       | FLOAT      { $$ = new ast::numerical<double>(yylval.fval); }
       | IDENTIFIER { $$ = new ast::identifier(yylval.sval);       }
;

comparator: OP_LT  { $$ = new ast::comparator(ast::comparator_type::LT);  }
          | OP_LTE { $$ = new ast::comparator(ast::comparator_type::LTE); }
          | OP_GT  { $$ = new ast::comparator(ast::comparator_type::GT);  }
          | OP_GTE { $$ = new ast::comparator(ast::comparator_type::GTE); }
          | OP_EQ  { $$ = new ast::comparator(ast::comparator_type::EQ);  }
          | OP_NEQ { $$ = new ast::comparator(ast::comparator_type::NEQ); }
;

%%

void yyerror(const char * /*message*/ ) {
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
