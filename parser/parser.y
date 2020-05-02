%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.h"
    #include "location.hh"

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0 "end of file"

    ASSIGN "="
    SEMICOLON ";"
    DOT "."
    COMMA ","

    // Ariphmetics
    MINUS "-"
    PLUS "+"
    STAR "*"
    SLASH "/"

    // Brackets
    LPAREN "("
    RPAREN ")"
    LBRACE "{"
    RBRACE "}"
    LBRACK "["
    RBRACK "]"

    // Builtins
    PUBLIC "public"
    STATIC "static"
    CLASS "class"

    // Simple types
    VOID "void"
    INT "int"

    // Builtin names
    MAIN "main"
    SYSTEM "System"
    OUT "out"
    PRINT "println"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"

%nterm <int> expr

%%
%start program;
program: main_class_decl {};

main_class_decl:
    "class" "identifier" "{" "public" "static" "void" "main" "(" ")" "{" statement_list "}" "}" {};

statement_list:
    %empty {}
    | statement_list statement {};

statement:
    var_decl {}
    | "System" "." "out" "." "println" "(" expr ")" ";" { std::cout << $7 << std::endl; }
    | "identifier" "=" expr ";"
        {
            if (driver.variables.count($1) > 0) {
                driver.variables[$1] = $3;
            } else {
                throw yy::parser::syntax_error(@1, "Unknown variable " + $1);
            }
        };

var_decl:
    "int" "identifier" ";" 
        {
            if (driver.variables.count($2) == 0)
                driver.variables[$2] = 0;
            else
                throw yy::parser::syntax_error(@$, "varible redefinition: " + $2);
        };

%left "+" "-";
%left "*" "/";

expr:
    "number"
    | "identifier"
        {
            if (driver.variables.count($1) > 0)
                $$ = driver.variables.at($1);
            else
                throw yy::parser::syntax_error(@1, "unknown variable " + $1);
        }
    | expr "+" expr { $$ = $1 + $3; }
    | expr "-" expr { $$ = $1 - $3; }
    | expr "*" expr { $$ = $1 * $3; }
    | expr "/" expr { $$ = $1 / $3; }
    | "(" expr ")" { $$ = $2; };

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
