
%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "y.tab.h"

  #define MAX_DEFINE 64

  void yyerror(char*);

  char** defineList = NULL;
  int nbOfStoredDefine = 0;
  int defineCalled = 0;

  int OUT_OF_DEFINE_USE_OR_DECL = 0;
  int DEFINE_DECLARATION = 1;
  int DEFINE_DECLARATION_BODY_EXPECTED = 2;
  int DEFINE_FOUND_PREPROCESSED_VAR_USE = 3;

%}
TYPE      int|stencil
STRING    \"([^\"]|\\.)*\"

IF_KW     if
ELSE_KW   else
WHILE_KW  while
FOR_KW    for
DEFINE    #define

NUMBER    0|([1-9][0-9]*)
ID        [a-zA-Z_]([a-zA-Z0-9_])*
OP        \+|-|\*|\/|\(|\)|\;|\,|\{|\}|\=|\<|\>|\[|\]|\$

INCR_OP   \+\+
DECR_OP   --
EQ_OP     \=\=
LEQ_OP    \<\=
GEQ_OP    \>\=
NOTEQ_OP  !\=
AND_OP    \&\&
OR_OP     \|\|
NOT_OP    !


%%

{DEFINE}   { defineCalled = 1;
             return DEFINE;
           }

{TYPE}     { yylval.string = strdup(yytext);
             return TYPE;
           }

{STRING}   { yylval.string = strdup(yytext);
             return STRING_LIT;
           }

{IF_KW}    { return IF; }

{ELSE_KW}  { return ELSE; }

{WHILE_KW} { return WHILE; }

{FOR_KW}   { return FOR; }

{OP}       { return yytext[0]; }

{DECR_OP}  { return DECR; }

{INCR_OP}  { return INCR; }

{EQ_OP}    { return EQ; }

{LEQ_OP}   { return LEQ; }

{GEQ_OP}   { return GEQ; }

{NOTEQ_OP} { return NOTEQ; }

{NOT_OP}   { return NOT; }

{AND_OP}   { return AND; }

{OR_OP}    { return OR; }

{NUMBER}   { yylval.val = atoi(yytext);
             return NUMBER;
           }

{ID}       { yylval.string = strdup(yytext);
             // Define declaration
             if(defineCalled == DEFINE_DECLARATION){
                if(nbOfStoredDefine == 0){
                  defineList = calloc(MAX_DEFINE , sizeof(char*));
                }
                defineList[nbOfStoredDefine] = yylval.string;
                nbOfStoredDefine++;
                defineCalled = DEFINE_DECLARATION_BODY_EXPECTED;
                return DEFINED;
             }
             else{
               // Checking if the ID is a defined one
               for(int i = 0; i < nbOfStoredDefine; i++){
                if( yylval.string != NULL && defineList[i] != NULL && strcmp(yylval.string, defineList[i]) == 0 ){
                  printf("ID got DEFINED\n");
                  defineCalled = DEFINE_FOUND_PREPROCESSED_VAR_USE;
                }
               }
               // If it is
               if(defineCalled == DEFINE_FOUND_PREPROCESSED_VAR_USE){
                 defineCalled = OUT_OF_DEFINE_USE_OR_DECL;
                 return DEFINED;
               }
               // Else
               return IDENTIFIER;
             }
           }


\n         { if(defineCalled == DEFINE_DECLARATION_BODY_EXPECTED){
                printf("prout\n");
                defineCalled = OUT_OF_DEFINE_USE_OR_DECL;
                return DEFINE_BODY_END;
             }
           }

[\n\t\ ]





%%
