
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "AST/quadToMIPS.h"
  #include "AST/define.h"

  int yylex();
  void yyerror(const char*);
  extern FILE *yyin;
  extern int yylineno;
  void yyfree();

  defineList* defList = NULL;

%}
%error-verbose

  // TYPE DEFINITIONS
%union{
  int val;
  char* string;
  struct s_ast* ast;

}

  // TYPES
%type <ast> defined_or_id;
%type <ast> expression;
%type <ast> statement;
%type <ast> statement_equal_value;
%type <ast> condition;
%type <ast> conditions_list;
%type <ast> loop_init;
%type <ast> loop;
%type <ast> instruction;
%type <ast> instructions_block;
%type <ast> arguments_declaration;
%type <ast> arguments_call;
%type <ast> function_declaration;
%type <ast> function_call;
%type <ast> curly_brackets;
%type <ast> curly_bracket_content_table;
%type <ast> brackets_table;
%type <ast> brackets_idx_value;
%type <ast> table_declaration;
%type <ast> table_access;
%type <ast> define_body;
%type <ast> stencil_declaration;
%type <ast> stencil_application;
%type <ast> stencil_call;
%type <ast> program;
%type <ast> axiom;

  // TOKENS
%token <val> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%token <string> STRING_LIT
%token <string> DEFINED
%token IF ELSE WHILE FOR DECR INCR EQ LEQ GEQ NOTEQ AND OR NOT DEFINE DEFINE_BODY_END RETURN

  // OPERATOR PRIORITIES
%left '+' '-'
%left '*' '/'
%left EQ LEQ GEQ NOTEQ
%left OR
%left AND
%left NOT
%left '(' ')'

%%

axiom:
  program                      {  $$ = $1;
                                  print_ast($$,0);
                                  symTable* symTableTest = genSymTable_init($$);
                                  symTable_print(symTableTest);
                                  codegen* cgBis = codegen_init();
                                  cgBis = codegen_ast(cgBis, $$, symTableTest);
                                  quadList_print(cgBis->code);
                                  genMIPS_genCode("test_quadToMIPS.test", cgBis, symTableTest);
                                  symTable_free(symTableTest);
                                  ast_free($$);
                                  codegen_free(cgBis);
                                  free_defineList(defList);
                                }
  ;



program:
  function_declaration program           { if($2 == NULL){
                                             fprintf(stderr, "preprocessed variable defined at the end of the file : please put it somewhere else\n");
                                             exit(1);
                                           }
                                           $$ = ast_concat($1, $2);
                                         }
  | define_declaration program           { $$ = $2; }
  | function_declaration                 { $$ = $1; }
  | define_declaration                   { $$ = NULL; }
  ;



define_declaration:
  DEFINE DEFINED define_body DEFINE_BODY_END            { if(defList == NULL){
                                                             defList = new_define($2, $3);
                                                           }
                                                           else{
                                                             defineList* d = search_existingDefine(defList, $2);
                                                             if(d == NULL){
                                                               add_define_to_list(defList, new_define($2, $3));
                                                             }
                                                             else{
                                                               printf("Second declaration of a unique defined value\n");
                                                               exit(1);
                                                             }
                                                           }
                                                         }

define_body:
  instructions_block          { $$ = $1; }
  | expression                { $$ = $1; }
  ;

stencil_declaration:
  TYPE IDENTIFIER '{' brackets_idx_value ',' brackets_idx_value '}' '=' '{' curly_bracket_content_table '}'   { int nbNeighbours = ($4->component.number*2)+1;
                                                                                                        int nbDim = $6->component.number;

                                                                                                        ast* tabDim = ast_new_tabDimension(ast_new_number(3));
                                                                                                        for(int i = 1; i < nbDim; i++){
                                                                                                          tabDim = ast_concat(tabDim, ast_new_tabDimension(ast_new_number(nbNeighbours)));
                                                                                                        }

                                                                                                        ast* stencil = ast_new_tabDeclaration(ast_new_identifier($2), tabDim, $10);
                                                                                                        $$ = stencil;
                                                                                                        free($1);
                                                                                                      }
  ;

stencil_application:
  stencil_call '$' table_access         { $$ = ast_new_binaryOperation(AST_OP_STEN, $1, $3); }
  | table_access '$' stencil_call         { $$ = ast_new_binaryOperation(AST_OP_STEN, $3, $1); }
  ;

stencil_call:
  IDENTIFIER      { $$ = ast_new_identifier($1); free($1); }
  ;



  table_declaration:
    TYPE IDENTIFIER brackets_table                                            { $$ = ast_new_tabDeclaration(ast_new_identifier($2), $3, NULL); free($1); free($2); }
    | TYPE IDENTIFIER brackets_table '=' '{' curly_bracket_content_table '}'  { $$ = ast_new_tabDeclaration(ast_new_identifier($2), $3, $6); free($1); free($2); }
    ;

  brackets_table:
    brackets_table '[' brackets_idx_value ']'   { $$ = ast_concat($1, ast_new_tabDimension($3)); }
    | '[' brackets_idx_value ']'                { $$ = ast_new_tabDimension($2); }
    ;

  brackets_idx_value:
    expression                        { $$ = $1; }
    | IDENTIFIER DECR                 { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1))); free($1); }
    | IDENTIFIER INCR                 { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1))); free($1); }
    | IDENTIFIER brackets_table DECR  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    | IDENTIFIER brackets_table INCR  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    ;

  curly_brackets:
    '{' curly_bracket_content_table '}' ',' curly_brackets                  { $$ = ast_concat(ast_new_tableElementsBlock($2), $5) ; }
    | '{' curly_bracket_content_table '}'                                   { $$ = ast_new_tableElementsBlock($2); }
    ;

  curly_bracket_content_table:
    expression ',' curly_bracket_content_table                              { $$ = ast_concat(ast_new_tabElements($1), $3); }
    | IDENTIFIER DECR ',' curly_bracket_content_table                       { $$ = ast_concat(ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1)))) , $4); free($1); }
    | IDENTIFIER INCR ',' curly_bracket_content_table                       { $$ = ast_concat(ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1)))) , $4); free($1); }
    | IDENTIFIER brackets_table DECR ',' curly_bracket_content_table        { $$ = ast_concat(ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($1), $2)))), $5); free($1); }
    | IDENTIFIER brackets_table INCR ',' curly_bracket_content_table        { $$ = ast_concat(ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($1), $2)))), $5); free($1); }
    | expression                                                            { $$ = ast_new_tabElements($1); }
    | IDENTIFIER DECR                                                       { $$ = ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1)))); free($1); }
    | IDENTIFIER INCR                                                       { $$ = ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1)))); free($1); }
    | IDENTIFIER brackets_table DECR                                        { $$ = ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($1), $2)))); free($1); }
    | IDENTIFIER brackets_table INCR                                        { $$ = ast_new_tabElements(ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($1), $2)))); free($1); }
    | curly_brackets                                                        { $$ = $1; }
    ;

  function_call:
    defined_or_id '(' arguments_call ')'                  { $$ = ast_new_functionCall($1, $3); }
    | defined_or_id '(' ')'                               { $$ = ast_new_functionCall($1, NULL); }
    ;

  function_declaration:
    TYPE IDENTIFIER '(' arguments_declaration ')' '{' instructions_block '}'        { $$ = ast_new_functionDefinition(ast_new_identifier($2), $4, $7); free($1); free($2); }
    | TYPE IDENTIFIER '(' ')' '{' instructions_block '}'                            { $$ = ast_new_functionDefinition(ast_new_identifier($2), NULL, $6); free($1); free($2); }
    ;

  arguments_call:
    defined_or_id ',' arguments_call                       { $$ = ast_concat(ast_new_argument($1), $3); }
    | STRING_LIT ',' arguments_call                        { $$ = ast_concat(ast_new_argument(ast_new_string($1)), $3); free($1); }
    | table_access ',' arguments_call                      { $$ = ast_concat(ast_new_argument($1), $3); }
    | NUMBER ','arguments_call                             { $$ = ast_concat(ast_new_argument(ast_new_number($1)), $3); }
    | defined_or_id                                        { $$ = ast_new_argument($1); }
    | STRING_LIT                                           { $$ = ast_new_argument(ast_new_string($1)); free($1); }
    | table_access                                         { $$ = ast_new_argument($1); }
    | NUMBER                                               { $$ = ast_new_argument(ast_new_number($1)); }
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
    | function_call ';'      { $$ = ast_new_Instruction($1); }
    | table_declaration ';'  { $$ = ast_new_Instruction($1); }
    | stencil_declaration ';'{ $$ = ast_new_Instruction($1); }
    ;

  loop:
    IF '(' conditions_list ')' '{' instructions_block '}'                                                         { placeGoto($3, $6, NULL); $$ = ast_new_controlStructure(AST_IF, $3, $6, NULL, NULL, NULL); }
    | IF '(' conditions_list ')' '{' instructions_block '}' ELSE '{' instructions_block '}'                       { placeGoto($3, $6, $10); $$ = ast_new_controlStructure(AST_IF, $3, $6, $10, NULL, NULL); }
    | WHILE '(' conditions_list ')' '{' instructions_block '}'                                                    { placeGoto($3, $6, NULL); $$ = ast_new_controlStructure(AST_WHILE, $3, $6, NULL, NULL, NULL); }
    | FOR '(' loop_init ';' conditions_list ';' IDENTIFIER brackets_table DECR ')' '{' instructions_block '}'     { placeGoto($5, $12, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $12, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($7), $8), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($7), $8)))); free($7); }
    | FOR '(' loop_init ';' conditions_list ';' IDENTIFIER brackets_table INCR ')' '{' instructions_block '}'     { placeGoto($5, $12, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $12, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($7), $8), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($7), $8)))); free($7); }
    | FOR '(' loop_init ';' conditions_list ';' table_access '=' expression ')' '{' instructions_block '}'        { placeGoto($5, $12, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $12, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, $7, $9)); }
    | FOR '(' loop_init ';' conditions_list ';' IDENTIFIER DECR ')' '{' instructions_block '}'                    { placeGoto($5, $11, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $11, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($7), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($7)))); free($7); }
    | FOR '(' loop_init ';' conditions_list ';' IDENTIFIER INCR ')' '{' instructions_block '}'                    { placeGoto($5, $11, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $11, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($7), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($7)))); free($7); }
    | FOR '(' loop_init ';' conditions_list ';' IDENTIFIER '=' expression ')' '{' instructions_block '}'          { placeGoto($5, $12, NULL); $$ = ast_new_controlStructure(AST_FOR, $5, $12, NULL, $3, ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($7), $9)); free($7); }
    ;

  loop_init:
    statement                 { $$ = $1; }
    | IDENTIFIER              { $$ = ast_new_identifier($1); free($1); }
    | table_access            { $$ = $1; }

  conditions_list:
    conditions_list AND conditions_list             { $1->component.boolean.ast_true = $3; }
      | conditions_list OR conditions_list          { $1->component.boolean.ast_false = $3; }
      | '(' conditions_list ')'                     { $$ = $2; }
      | NOT conditions_list                         { $$ = ast_new_boolExpr($2->component.boolean.boolExpr, $2->component.boolean.ast_false, $2->component.boolean.ast_true); free($2); }
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

  statement_equal_value:
    expression                        { $$ = $1; }
    | IDENTIFIER DECR                 { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1))); free($1); }
    | IDENTIFIER INCR                 { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1))); free($1); }
    | IDENTIFIER brackets_table DECR  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    | IDENTIFIER brackets_table INCR  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    ;


  statement:
    IDENTIFIER '=' statement_equal_value         { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), $3); free($1); }
    | TYPE IDENTIFIER                            { $$ = ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)); free($1); free($2); }
    | TYPE IDENTIFIER '=' statement_equal_value  { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier($2)), $4); free($1); free($2); }
    | IDENTIFIER DECR                            { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_DECR, ast_new_identifier($1))); free($1); }
    | IDENTIFIER INCR                            { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_identifier($1), ast_new_unaryOperation(AST_OP_INCR, ast_new_identifier($1))); free($1); }
    | table_access '=' statement_equal_value     { $$ = ast_new_binaryOperation(AST_OP_AFCT, $1, $3); }
    | IDENTIFIER brackets_table DECR             { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_DECR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    | IDENTIFIER brackets_table INCR             { $$ = ast_new_binaryOperation(AST_OP_AFCT, ast_new_tableAccess(ast_new_identifier($1), $2), ast_new_unaryOperation(AST_OP_INCR, ast_new_tableAccess(ast_new_identifier($1), $2))); free($1); }
    ;


  expression:
    expression '+' expression   { $$ = ast_new_binaryOperation(AST_OP_ADD, $1, $3); }
    | expression '-' expression { $$ = ast_new_binaryOperation(AST_OP_SUB, $1, $3); }
    | expression '*' expression { $$ = ast_new_binaryOperation(AST_OP_MULT, $1, $3); }
    | expression '/' expression { $$ = ast_new_binaryOperation(AST_OP_DIV, $1, $3); }
    | '(' expression ')'        { $$ = $2; }
    | '-' expression            { $$ = ast_new_unaryOperation(AST_OP_MINUS, $2); }
    | NUMBER                    { $$ = ast_new_number($1); }
    | defined_or_id             { $$ = $1; }
    | table_access              { $$ = $1; }
    | stencil_application       { $$ = $1; }
    ;


  defined_or_id:
    IDENTIFIER                  { $$ = ast_new_identifier($1); free($1); }
    | DEFINED                   { defineList* temp = search_existingDefine(defList, $1);
                                  if(temp != NULL){
                                    $$ = ast_copy(temp->value);
                                    free($1);
                                  }
                                  else{
                                    printf("Error : undefined value %s\n", $1);
                                    exit(1);
                                  }

                                }
    ;

  table_access:
    IDENTIFIER brackets_table           { $$ = ast_new_tableAccess(ast_new_identifier($1), $2); free($1); }
    ;

  %%

  void yyerror(const char *s) {
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
