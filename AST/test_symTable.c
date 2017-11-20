#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

  symTable* test = symTable_init();

  symTable_newTemp(test, 42);

  symTable_addConst(test, "lol", 17);

  symTable_newTemp(test, 50);

  symTable_newTemp(test, 67);

  symTable_addConst(test, "const", 54);

  symTable_addConst(test, "const", 72);

  symTable_print(test);

  symTable_free(test);

  return 0;
}
