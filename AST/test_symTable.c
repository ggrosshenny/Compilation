#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{

  symTable* test = symTable_init(NULL);

  symbol* testSymbol = NULL;

  testSymbol = symTable_newTemp(test, 42);
  assert(strcmp(testSymbol->identifier, "$temp_0")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->value == 42);

  testSymbol = symTable_addConst(test, "lol");
  assert(strcmp(testSymbol->identifier, "lol")==0);
  assert(testSymbol->isConstant == true);

  testSymbol = symTable_newTemp(test, 50);
  assert(strcmp(testSymbol->identifier, "$temp_1")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->value == 50);

  testSymbol = symTable_newTemp(test, 67);
  assert(strcmp(testSymbol->identifier, "$temp_2")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->value == 67);

  testSymbol = symTable_addConst(test, "const");
  assert(strcmp(testSymbol->identifier, "const")==0);
  assert(testSymbol->isConstant == true);

  testSymbol = symTable_addConst(test, "const");
  assert(testSymbol == NULL);

  symTable_print(test);

  symTable_free(test);

  return 0;
}
