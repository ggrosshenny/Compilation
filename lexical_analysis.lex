
%{
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"
  void yyerror(char*);

%}
TYPE      int
STRING    \"([^\"]|\\.)*\"

IF_KW     if
ELSE_KW   else
WHILE_KW  while
FOR_KW    for
DEFINE    \#(define|DEFINE)

NUMBER    0|([1-9][0-9]*)
ID        [a-zA-Z_]([a-zA-Z0-9_])*
OP        \+|-|\*|\/|\(|\)|\;|\,|\{|\}|\=|\<|\>|\[|\]

INCR_OP   \+\+
DECR_OP   --
EQ_OP     \=\=
LEQ_OP    \<\=
GEQ_OP    \>\=
NOTEQ_OP  !\=
AND_OP    \&\&
OR_OP     \|\|
NOT_OP  !


%%

{DEFINE}

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
             return IDENTIFIER;
           }



[\n\t\ ]


%%
