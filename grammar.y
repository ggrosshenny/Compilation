
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "AST/quadToMIPS.h"

  int yylex();
  void yyerror(char*);
  extern FILE *yyin;
  extern int yylineno;

%}
  // TYPE DEFINITIONS
%union{
  int val;
  char* string;
  struct s_ast* ast;

}

  // TYPES
%type <ast> expression;
%type <ast> axiom;

  // TOKENS
%token <val> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%token <string> KEYWORDS

  // OPERATOR PRIORITIES
%left '+' '-'
%left '*' '/'
%left '(' ')'

%%
axiom:
  expression '\n'      {  print_ast($$,0);
                          symTable* symTableTest = symTable_init();

                          genSymTable_ast($$, symTableTest);
                          codegen* cgBis = codegen_init();
                          cgBis = codegen_ast(cgBis, $$, symTableTest);

                          genMIPS_genCode("test_quadToMIPS.test", cgBis->code, symTableTest);

                          symTable_free(symTableTest);
                          ast_free($$);
                          codegen_free(cgBis);

                          exit(0);
                       }
  ;

expression:
  expression '+' expression   { $$ = ast_new_binaryOperation(AST_OP_ADD, $1, $3); }
  | expression '-' expression { $$ = ast_new_binaryOperation(AST_OP_SUB, $1, $3); }
  | expression '*' expression { $$ = ast_new_binaryOperation(AST_OP_MULT, $1, $3); }
  | expression '/' expression { $$ = ast_new_binaryOperation(AST_OP_DIV, $1, $3); }
  | '(' expression ')'        { $$ = $2; }
  | '-' expression            { $$ = ast_new_unaryOperation(AST_OP_MINUS, $2); }
  | IDENTIFIER "--"           { $$ = ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1)); }
  | IDENTIFIER "++"           { $$ = ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1)); }
  | NUMBER                    { $$ = ast_new_number($1); }
  | IDENTIFIER                { $$ = ast_new_identifier($1); }
  ;

%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}

int main(int argc, char** argv){
  yyin = fopen(argv[1], "r");
  yylex();
  fclose(yyin);
  return yyparse();
}
