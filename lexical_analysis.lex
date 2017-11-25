
%{
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"
  void yyerror(char*);

%}
TYPE      int
KEYWORDS  if|else|while|for|main|printf|printi
NUMBER    0|([1-9][0-9]*)
ID        [a-zA-Z_]([a-zA-Z0-9_])*
OP        \+|-|\+\+|--|\*|\/|\(|\)|\;|=|==|\<|\<=|\>|\>=|!=

%%

{TYPE}     { yylval.string = strdup(yytext);
             return TYPE;
           }

{KEYWORDS} { yylval.string = strdup(yytext);
             return KEYWORDS;
           }

{NUMBER}   { yylval.val = atoi(yytext);
             return NUMBER;
           }

{ID}       { yylval.string = strdup(yytext);
             return IDENTIFIER;
           }

{OP}       { return yytext[0]; }

[\n{}]     { return yytext[0]; }


%%
/*
int main(int argc, char** argv){

  yyin = fopen(argv[1], "r");
  yylex();
  fclose(yyin);
  return (0);
}
*/
