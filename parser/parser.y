%skeleton "lalr1.cc"
%require "3.5"

%expect 0

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include <program/visitor.h>
    class Scanner;
    class Driver;
}

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

    // Ariphmetics
    MINUS "-"
    PLUS "+"
    STAR "*"
    SLASH "/"
    MODULO "%"

    // Logic
    AND "&&"
    OR "||"
    NEG "!"
    GREATER ">"
    LESS "<"
    GE ">="
    LE "<="
    EQUAL "=="
    NE "!="

    // Other operations
    ASSIGN "="
    SEMICOLON ";"
    DOT "."
    COMMA ","

    // Brackets
    LPAREN "("
    RPAREN ")"
    LBRACE "{"
    RBRACE "}"
    LBRACK "["
    RBRACK "]"
    BRACKS "[]"

    // Builtins
    PUBLIC "public"
    STATIC "static"
    CLASS "class"
    EXTENDS "extends"

    // Branching
    IF "if"
    ELSE "else"
    WHILE "while"

    // Control transfer
    RETURN "return"

    // Simple types
    VOID "void"
    INT "int"
    BOOL "boolean"

    // Builtin names
    MAIN "main"
    SYSTEM "System"
    OUT "out"
    PRINT "println"
    ASSERT "assert"
    LENGTH "length"
    NEW "new"
    THIS "this"
    TRUE "true"
    FALSE "false"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"

%nterm <std::shared_ptr<Program>> program
%nterm <std::shared_ptr<MainClassDecl>> main_class_decl
%nterm <std::shared_ptr<ClassDeclList>> class_decl_list
%nterm <std::shared_ptr<ClassDeclaration>> class_decl
%nterm <std::shared_ptr<DeclarationList>> decl_list
%nterm <std::shared_ptr<Declaration>> decl
%nterm <std::shared_ptr<MethodDecl>> method_decl
%nterm <std::shared_ptr<VariableDecl>> var_decl
%nterm <std::shared_ptr<ArgumentDeclList>> argument_list
%nterm <std::shared_ptr<ArgumentDecl>> argument
%nterm <std::shared_ptr<Type>> type
%nterm <std::shared_ptr<SimpleType>> simple_type
%nterm <std::shared_ptr<StatementList>> statement_list
%nterm <std::shared_ptr<Statement>> statement
%nterm <std::shared_ptr<Statement>> if_else_statement
%nterm <std::shared_ptr<Statement>> uncond_statement
%nterm <std::shared_ptr<MethodCall>> method_call
%nterm <std::shared_ptr<ArgumentValues>> argument_values
%nterm <std::shared_ptr<LValue>> lvalue
%nterm <std::shared_ptr<Expression>> expr
%nterm <std::shared_ptr<CallableExpr>> callable_expr
%nterm <std::shared_ptr<BooleanExpr>> boolean_expr

%%
%start program;
%left "||";
%left "&&";
%left ">" "<" ">=" "<=" "==" "!=";
%left "+" "-";
%left "*" "/" "%";
%right "!";

program: main_class_decl class_decl_list { $$ = std::make_shared<Program>($1, $2); driver.program = $$; };

main_class_decl:
    "class" "identifier" "{"
        "public" "static" "void" "main" "(" ")" "{"
            statement_list
        "}"
    "}" { $$ = std::make_shared<MainClassDecl>($2, $11); };

class_decl_list:
    %empty { $$ = std::make_shared<ClassDeclList>(); }
    |
    class_decl_list class_decl { $$ = $1; $$->AddDeclaration($2); };

class_decl:
    "class" "identifier" "{"
        decl_list
    "}" { $$ = std::make_shared<ClassDeclaration>($2, $4); }
    |
    "class" "identifier" "extends" "identifier" "{"
        decl_list
    "}" { $$ = std::make_shared<ClassDeclaration>($2, $4, $6); };

decl_list:
    %empty { $$ = std::make_shared<DeclarationList>(); }
    |
    decl_list decl { $$ = $1; $$->AddDeclaration($2); };

decl:
    var_decl { $$ = $1; }
    |
    method_decl { $$ = $1; };

method_decl:
    "public" type "identifier" "(" argument_list ")" "{"
        statement_list
    "}" { $$ = std::make_shared<MethodDecl>($2, $3, $5, $8); }
    |
    "public" type "identifier" "(" ")" "{"
        statement_list
    "}" { $$ = std::make_shared<MethodDecl>($2, $3, $7); };

var_decl:
    type "identifier" ";" { $$ = std::make_shared<VariableDecl>($1, $2); };

argument_list:
    argument { $$ = std::make_shared<ArgumentDeclList>(); $$->AddArgumentDecl($1); }
    |
    argument_list "," argument { $1->AddArgumentDecl($3); $$ = $1; };

argument:
    type "identifier" { $$ = std::make_shared<ArgumentDecl>($1, $2); };

type:
    simple_type { $$ = $1; }
    |
    simple_type "[]" { $$ = std::make_shared<ArrayType>($1); };

simple_type:
    "int" { $$ = std::make_shared<SimpleType>("int"); }
    |
    "boolean" { $$ = std::make_shared<SimpleType>("boolean"); }
    |
    "void" { $$ = std::make_shared<SimpleType>("void"); }
    |
    "identifier" { $$ = std::make_shared<SimpleType>($1); };

statement_list:
    %empty { $$ = std::make_shared<StatementList>(); }
    |
    statement_list statement { $$ = $1; $$->AddStatement($2); };

statement:
    "if" "(" boolean_expr ")" statement { $$ = std::make_shared<ConditionalStatement>($3, $5); }
    |
    "if" "(" boolean_expr ")"
        if_else_statement
    "else"
        statement
    { $$ = std::make_shared<ConditionalStatement>($3, $5, $7); }
    |
    "while" "(" boolean_expr ")" statement
    { $$ = std::make_shared<WhileStatement>($3, $5); }
    |
    uncond_statement
    { $$ = $1; };

if_else_statement:
    "if" "(" boolean_expr ")"
        if_else_statement
    "else"
        if_else_statement
    { $$ = std::make_shared<ConditionalStatement>($3, $5, $7); }
    |
    uncond_statement
    { $$ = $1; };

uncond_statement:
    "assert" "(" boolean_expr ")" ";"
    { $$ = std::make_shared<Assert>($3); }
    |
    var_decl
    { $$ = std::make_shared<VariableStatement>($1); }
    |
    "{" statement_list "}"
    { $$ = $2; }
    |
    "System" "." "out" "." "println" "(" expr ")" ";"
    { $$ = std::make_shared<PrintStatement>($7); }
    |
    lvalue "=" expr ";"
    { $$ = std::make_shared<Assignment>($1, $3); }
    |
    "return" expr ";"
    { $$ = std::make_shared<ReturnStatement>($2); }
    |
    method_call ";"
    { $$ = std::make_shared<CallStatement>($1); };

method_call:
    callable_expr "." "identifier" "(" ")"
    { $$ = std::make_shared<MethodCall>($1, $3); }
    |
    callable_expr "." "identifier" "(" argument_values ")"
    { $$ = std::make_shared<MethodCall>($1, $3, $5); };

argument_values:
    expr
    { $$ = std::make_shared<ArgumentValues>(); $$->AddValue($1); }
    |
    argument_values "," expr
    { $$ = $1; $1->AddValue($3); };

lvalue:
    "identifier"
    { $$ = std::make_shared<Variable>($1); }
    |
    "identifier" "[" expr "]"
    { $$ = std::make_shared<ArrayElement>($1, $3); };

expr:
    expr "+" expr
    { $$ = std::make_shared<PlusExpr>($1, $3); }
    |
    expr "-" expr
    { $$ = std::make_shared<MinusExpr>($1, $3); }
    |
    expr "*" expr
    { $$ = std::make_shared<MulExpr>($1, $3); }
    |
    expr "/" expr
    { $$ = std::make_shared<DivideExpr>($1, $3); }
    |
    expr "%" expr
    { $$ = std::make_shared<RemainderExpr>($1, $3); }
    |
    "(" expr ")"
    { $$ = $2; }
    |
    callable_expr "." "length"
    { $$ = std::make_shared<LengthExpr>($1); }
    |
    "number"
    { $$ = std::make_shared<NumberExpr>($1); }
    |
    callable_expr
    { $$ = $1; };

callable_expr:
    lvalue
    { $$ = std::make_shared<LValueExpr>($1); }
    |
    "new" simple_type "[" expr "]"
    { $$ = std::make_shared<ArrayAllocation>($2, $4); }
    |
    "new" simple_type "(" ")"
    { $$ = std::make_shared<Allocation>($2); }
    |
    "this"
    { $$ = std::make_shared<ThisExpr>(); }
    |
    method_call
    { $$ = std::make_shared<CallExpr>($1); };

boolean_expr:
    boolean_expr "||" boolean_expr
    { $$ = std::make_shared<OrExpr>($1, $3); }
    |
    boolean_expr "&&" boolean_expr
    { $$ = std::make_shared<AndExpr>($1, $3); }
    |
    "!" boolean_expr
    { $$ = std::make_shared<NotExpr>($2); }
    |
    "(" boolean_expr ")"
    { $$ = $2; }
    |
    expr ">" expr
    { $$ = std::make_shared<GreaterExpr>($1, $3); }
    |
    expr "<" expr
    { $$ = std::make_shared<LessExpr>($1, $3); }
    |
    expr ">=" expr
    { $$ = std::make_shared<GreaterEqualExpr>($1, $3); }
    |
    expr "<=" expr 
    { $$ = std::make_shared<LessEqualExpr>($1, $3); }
    |
    expr "==" expr
    { $$ = std::make_shared<EqualExpr>($1, $3); }
    |
    expr "!=" expr
    { $$ = std::make_shared<NotEqualExpr>($1, $3); }
    |
    "true"
    { $$ = std::make_shared<TrueExpr>(); }
    |
    "false"
    { $$ = std::make_shared<FalseExpr>(); };

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
