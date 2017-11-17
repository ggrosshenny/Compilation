#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

  symTable* test = symTable_init();

  symTable_newTemp(test, 42);

  symTable_addConst(test, "lol", 17);

  symTable_free(test);

  return 0;
}
