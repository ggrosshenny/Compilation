
%{
  #include <stdlib.h>
  #include "y.tab.h"

%}
NUMBER    0|([1-9][0-9]*)
ID        [a-zA-Z_][a-zA-Z0-9_]*
OP        \+|\-|\+\+|\-\-|\*|\/|\(|\)
TYPE      int
KEYWORDS  if|else|while|for


%%

{NUMBER}   { yylval.val = atoi(yytext);
           return NOMBRE;
}

{ID}       { yylval.float = yytext;
           return ID;
}

{OP}       { return yytext[0]; }

{TYPE}     { return TYPE; }

{KEYWORDS} { return KEYWORDS; }

[\ \t\n]   { return yytext; }



%%
