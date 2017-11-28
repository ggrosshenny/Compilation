
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
%type <ast> statement;
%type <ast> instruction;
%type <ast> instructions_block;
%type <ast> arguments;
%type <ast> function;
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
  function                      {  print_ast($$,0);

                                  symTable* symTableTest = symTable_init($$);

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

function:
  TYPE IDENTIFIER '(' arguments ')' '{' instructions_block '}'        { $$ = ast_new_functionDefinition(ast_new_identifier($2), $4, $7); }
  ;

arguments:
  IDENTIFIER ',' arguments                  { $$ = ast_concat(ast_new_identifier($1),$3); }
  | IDENTIFIER                          { $$ = ast_new_identifier($1); }
  ;

instructions_block:
  instruction instructions_block        { $$ = ast_concat($1, $2); }
  | instruction                         { $$ = $1; }
  ;

instruction:
  statement                             { $$ = ast_new_Instruction($1); }
  ;

statement:
  IDENTIFIER '=' expression ';'         { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), $3); }
  | TYPE IDENTIFIER ';'                 { $$ = ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($1)); }
  | TYPE IDENTIFIER '=' expression ';'  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)), $4); }
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
  do{
    yyparse();
  } while (!feof(yyin));
  fclose(yyin);
  return 0;
}
