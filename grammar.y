

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "AST/ast.h"
  int yylex();
  void yyerror(char*);
  extern int yylineno;

%}

%union{
  int val;
  char* string;
  struct s_ast* ast;
}

%type <ast> expression;
%type <ast> axiom;

  // TOKENS & OPERATOR PRIORITIES
%token <val> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%token <string> KEYWORDS
%left '+' '-'
%left '*' '/'
%left '(' ')'





%%
axiom:
  expression '\n'              { print_ast($$, 1);
                                exit(0);
                              }
  ;

statement:
  IDENTIFIER '=' expression ';'         { printf("id=expr;"); }
  | TYPE IDENTIFIER '=' expression ';'  { printf("type id=expr"); }
  ;

expression:
  expression '+' expression   { $$ = ast_new_binaryOperation(AST_OP_ADD, $1, $3); }
  | expression '-' expression { $$ = ast_new_binaryOperation(AST_OP_MINUS, $1, $3); }
  | expression '*' expression { $$ = ast_new_binaryOperation(AST_OP_MULT, $1, $3); }
  | expression '/' expression { $$ = ast_new_binaryOperation(AST_OP_DIV, $1, $3); }
  | '(' expression ')'        { $$ = $2; }
  | '-' expression            { $$ = ast_new_unaryOperation(AST_OP_MINUS, $2); }
  | expression "--"           { $$ = ast_new_unaryOperation(AST_OP_DECR, $1); }
  | expression "++"           { $$ = ast_new_unaryOperation(AST_OP_INCR, $1); }
  | NUMBER                    { $$ = ast_new_number($1); }
  | IDENTIFIER                { $$ = ast_new_identifier($1); }
  ;


%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}

int main(){
  return yyparse();
}
