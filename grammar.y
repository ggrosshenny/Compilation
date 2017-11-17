

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
}

  // TOKENS & OPERATOR PRIORITIES
%token <val> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%token <string> KEYWORDS
%left '+' '-'
%left '*' '%'
%left '(' ')'



%%
axiom:
  statement '\n'              { printf("good\n");
                                exit(0);
                              }
  ;

statement:
  IDENTIFIER '=' expression ';'         { printf("id=expr");}
  | TYPE IDENTIFIER '=' expression ';'  { printf("type id=expr"); }
  ;

expression:
  expression '+' expression   { printf("expr+expr"); }
  | expression '-' expression { printf("expr-expr"); }
  | expression '*' expression { printf("expr*expr"); }
  | '(' expression ')'        { printf("(expr)"); }
  | '-' expression            { printf("-expr"); }
  | NUMBER                    { printf("NUMBER"); }
  | IDENTIFIER                { printf("ID"); }
  ;


%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}

int main(){
  return yyparse();
}
