%{

#include <iostream>
#include "nodes.hpp"
#include "output.hpp"
#include "token.hpp"

// bison declarations
extern int yylineno;
extern int yylex();

void yyerror(const char*);

// root of the AST, set by the parser and used by other parts of the compiler
std::shared_ptr<ast::Node> program; // "program" is the rootNode

using namespace std;

// TODO: Place any additional declarations here
using namespace output;

%}

// TODO: Define tokens here
%union {
    shared_ptr<ast::Node> astNode;
    shared_ptr<ast::ID> identifier;
    shared_ptr<ast::Exp> expression;
}

%token <astNode> VOID INT BYTE BOOL AND OR NOT TRUE FALSE RETURN IF ELSE WHILE BREAK CONTINUE
%token SC COMMA LPAREN RPAREN LBRACE RBRACE
%token ASSIGN RELOP BINOP
%token <identifier> ID
%token <expression> NUM NUM_B STRING

// TODO: Define precedence and associativity here
%left LPAREN RPAREN
%right '+' '-'
%right NOT
%left '*' '/'
%left RELOP
%left AND OR
%right ASSIGN

%type <astNode> Program Funcs FuncDecl RetType Formals FormalsList FormalDecl Statements Statement Call ExpList Type Exp

%%

// While reducing the start variable, set the root of the AST
Program:  Funcs { program = $1; }
;

// TODO: Define grammar here

%%

// TODO: Place any additional code here
