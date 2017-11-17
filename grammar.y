

%{
  #include <stdio.h>
  #include <stdlib.h>
  int yylex();
  void yyerror(char*);

%}

  // TOKENS & OPERATOR PRIORITIES
%token NUMBER ID TYPE KEYWORDS
%left '+' '-' '++' '--'
%left '*' '%'
%left '(' ')'

  // STUCTURES
%union{
  struct {
    int val;
  } integer;


  struct {
    char* id_name;
    int val;
  } identifier;

  char* keyword;

%type <integer> NUMBER;
%type <identifier> ID;

}


%%
axiom:

  ;

statement:
  IDENTIFIER '=' expression ';'
  | TYPE IDENTIFIER '=' expression ';'
  ;

expression:
  expression '+' expression
  | expression '-' expression
  | expression '*' expression
  | '(' expression ')'
  | NUMBER
  | IDENTIFIER
  ;


%%
