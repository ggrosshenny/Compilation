
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "AST/quadToMIPS.h"

  int yylex();
  void yyerror(char*);
  extern FILE *yyin;
  extern int yylineno;
  void yyfree();

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
%token <string> STRING_LIT
%token IF ELSE WHILE FOR DECR INCR EQ LEQ GEQ NOTEQ AND OR

  // OPERATOR PRIORITIES
%left '+' '-'
%left '*' '/'
%left EQ LEQ GEQ NOTEQ
%left OR
%left AND
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
                                  ast_free($$);
                                  exit(0);
                                }
  ;

function:
  function_declaration function           { $$ = ast_concat($1, $2); }
  | function_declaration                  { $$ = $1; }
  ;

function_call:
  IDENTIFIER '(' arguments_call ')' ';'                  { $$ = ast_new_functionCall(ast_new_identifier($1), $3); free($1); }
  | IDENTIFIER '(' ')' ';'                               { $$ = ast_new_functionCall(ast_new_identifier($1), NULL); free($1); }
  ;

function_declaration:
  TYPE IDENTIFIER '(' arguments_declaration ')' '{' instructions_block '}'        { $$ = ast_new_functionDefinition(ast_new_identifier($2), $4, $7); free($1); free($2); }
  | TYPE IDENTIFIER '(' ')' '{' instructions_block '}'                            { $$ = ast_new_functionDefinition(ast_new_identifier($2), NULL, $6); free($1); free($2); }
  ;

arguments_call:
  IDENTIFIER ',' arguments_call                          { $$ = ast_concat(ast_new_argument(ast_new_identifier($1)), $3); free($1); }
  | STRING_LIT ',' arguments_call                        { $$ = ast_concat(ast_new_argument(ast_new_string($1)), $3); free($1); }
  | IDENTIFIER                                           { $$ = ast_new_argument(ast_new_identifier($1)); free($1); }
  | STRING_LIT                                           { $$ = ast_new_argument(ast_new_string($1)); free($1); }
  ;

arguments_declaration:
  TYPE IDENTIFIER ',' arguments_declaration              { $$ = ast_concat(ast_new_argument(ast_new_identifier($2)), $4); free($1); free($2); }
  | TYPE IDENTIFIER                                      { $$ = ast_new_argument(ast_new_identifier($2)); free($1); free($2); }
  ;

instructions_block:
  instruction instructions_block        { $$ = ast_concat($1, $2); }
  | instruction                         { $$ = $1; }
  ;

instruction:
  statement ';'            { $$ = ast_new_Instruction($1); }
  | loop                   { $$ = ast_new_Instruction($1); }
  | function_call          { $$ = ast_new_Instruction($1); }
  ;

loop:
  IF '(' conditions_list ')' '{' instructions_block '}'                                                         { placeGoto($3, $6, NULL); $$ = ast_new_controlStructure(AST_IF, $3, $6, NULL, NULL, NULL); }
  | IF '(' conditions_list ')' '{' instructions_block '}' ELSE '{' instructions_block '}'                       { placeGoto($3, $6, $10); $$ = ast_new_controlStructure(AST_IF, $3, $6, $10, NULL, NULL); }
  | WHILE '(' conditions_list ')' '{' instructions_block '}'                                                    { placeGoto($3, $6, NULL); $$ = ast_new_controlStructure(AST_WHILE, $3, $6, NULL, NULL, NULL); }
  | FOR '(' statement ';' conditions_list ';' IDENTIFIER DECR ')' '{' instructions_block '}'                    { placeGoto($5, $11, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $11, NULL, $3, ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($7))); free($7); }
  | FOR '(' statement ';' conditions_list ';' IDENTIFIER INCR ')' '{' instructions_block '}'                    { placeGoto($5, $11, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $11, NULL, $3, ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($7))); free($7); }
  | FOR '(' statement ';' conditions_list ';' IDENTIFIER '=' expression ')' '{' instructions_block '}'          { placeGoto($5, $12, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $12, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($7), $9)); free($7); }
  ;

conditions_list:
  conditions_list AND conditions_list             { $1->component.boolean.ast_true = $3; }
    | conditions_list OR conditions_list          { $1->component.boolean.ast_false = $3; }
    | '(' conditions_list ')'                     { $$ = ast_new_boolExpr($2, NULL, NULL); }
    | condition                                   { $$ = ast_new_boolExpr($1, NULL, NULL); }
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
  IDENTIFIER '=' expression         { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), $3); free($1); }
  | TYPE IDENTIFIER                 { $$ = ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)); free($1); free($2); }
  | TYPE IDENTIFIER '=' expression  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)), $4); free($1); free($2); }
  | IDENTIFIER DECR                 { $$ = ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1)); free($1); }
  | IDENTIFIER INCR                 { $$ = ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1)); free($1); }
  ;


expression:
  expression '+' expression   { $$ = ast_new_binaryOperation(AST_OP_ADD, $1, $3); }
  | expression '-' expression { $$ = ast_new_binaryOperation(AST_OP_SUB, $1, $3); }
  | expression '*' expression { $$ = ast_new_binaryOperation(AST_OP_MULT, $1, $3); }
  | expression '/' expression { $$ = ast_new_binaryOperation(AST_OP_DIV, $1, $3); }
  | '(' expression ')'        { $$ = $2; }
  | '-' expression            { $$ = ast_new_unaryOperation(AST_OP_MINUS, $2); }
  | NUMBER                    { $$ = ast_new_number($1); }
  | IDENTIFIER                { $$ = ast_new_identifier($1); free($1); }
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
  yyfree();
  return 0;
}
