
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
%type <ast> condition;
%type <ast> conditions_list;
%type <ast> loop;
%type <ast> instruction;
%type <ast> instructions_block;
%type <ast> arguments_declaration;
%type <ast> arguments_call;
%type <ast> function_declaration;
%type <ast> function_call;
%type <ast> function;
%type <ast> axiom;

  // TOKENS
%token <val> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%token <string> KEYWORDS
%token IF ELSE WHILE FOR DECR INCR EQ LEQ GEQ NOTEQ AND OR

  // OPERATOR PRIORITIES
%left '+' '-'
%left '*' '/'
%left '(' ')'

%%
axiom:
  function                      {  print_ast($$,0);
/*
                                  symTable* symTableTest = symTable_init($$);

                                  genSymTable_ast($$, symTableTest);
                                  codegen* cgBis = codegen_init();
                                  cgBis = codegen_ast(cgBis, $$, symTableTest);

                                  genMIPS_genCode("test_quadToMIPS.test", cgBis->code, symTableTest);

                                  symTable_free(symTableTest);
                                  ast_free($$);
                                  codegen_free(cgBis);
*/
                                  exit(0);
                                }
  ;

function:
  function_declaration function           { $$ = ast_concat($1, $2); }
  | function_declaration                  { $$ = $1; }
  ;

function_call:
  IDENTIFIER '(' arguments_call ')' ';'                  { $$ = ast_new_functionCall(ast_new_identifier($1), $3); }
  ;

function_declaration:
  TYPE IDENTIFIER '(' arguments_declaration ')' '{' instructions_block '}'        { $$ = ast_new_functionDefinition(ast_new_identifier($2), $4, $7); }
  ;

arguments_call:
  IDENTIFIER ',' arguments_call                          { $$ = ast_concat(ast_new_argument(ast_new_identifier($1)), $3); }
  | IDENTIFIER                                           { $$ = ast_new_argument(ast_new_identifier($1)); }
  ;

arguments_declaration:
  TYPE IDENTIFIER ',' arguments_declaration              { $$ = ast_concat(ast_new_identifier($2), $4); }
  | TYPE IDENTIFIER                                      { $$ = ast_new_identifier($2); }
  ;

instructions_block:
  instruction instructions_block        { $$ = ast_concat($1, $2); }
  | instruction                         { $$ = $1; }
  ;

instruction:
  statement ';'            { $$ = ast_new_Instruction($1); }
  | loop                   { $$ = ast_new_Instruction($1); }
  | function_call  ';'        { $$ = ast_new_Instruction($1); }
  ;

loop:
  IF '(' conditions_list ')' '{' instructions_block '}'                                           { $$ = ast_new_boolExpr($3, $6, NULL); }
  | IF '(' conditions_list ')' '{' instructions_block '}' ELSE '{' instructions_block '}'         { $$ = ast_new_boolExpr($3, $6, $10); }
  | WHILE '(' conditions_list ')' '{' instructions_block '}'                                      {}
  | FOR '(' statement ';' conditions_list ';' statement ')' '{' instructions_block '}'            {}
  ;

conditions_list:
  conditions_list AND conditions_list             {}
    | conditions_list OR conditions_list          {}
    | '(' conditions_list ')'                     {}
    | condition                                   { $$ = $1; }
    ;

condition:
  expression EQ expression        { $$ = ast_new_binaryOperation(AST_BOOL_EQ, $1, $3); }
  | expression NOTEQ expression   { $$ = ast_new_binaryOperation(AST_BOOL_NEQ, $1, $3); }
  | expression LEQ expression     { $$ = ast_new_binaryOperation(AST_BOOL_LEQ, $1, $3); }
  | expression GEQ expression     { $$ = ast_new_binaryOperation(AST_BOOL_GEQ, $1, $3); }
  | expression '<' expression     { $$ = ast_new_binaryOperation(AST_BOOL_LT, $1, $3); }
  | expression '>' expression     { $$ = ast_new_binaryOperation(AST_BOOL_GT, $1, $3); }
  ;

statement:
  IDENTIFIER '=' expression         { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), $3); }
  | TYPE IDENTIFIER                 { $$ = ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)); }
  | TYPE IDENTIFIER '=' expression  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)), $4); }
  | IDENTIFIER DECR                 { $$ = ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1)); }
  | IDENTIFIER INCR                 { $$ = ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1)); }
  ;


expression:
  expression '+' expression   { $$ = ast_new_binaryOperation(AST_OP_ADD, $1, $3); }
  | expression '-' expression { $$ = ast_new_binaryOperation(AST_OP_SUB, $1, $3); }
  | expression '*' expression { $$ = ast_new_binaryOperation(AST_OP_MULT, $1, $3); }
  | expression '/' expression { $$ = ast_new_binaryOperation(AST_OP_DIV, $1, $3); }
  | '(' expression ')'        { $$ = $2; }
  | '-' expression            { $$ = ast_new_unaryOperation(AST_OP_MINUS, $2); }
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
