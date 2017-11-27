#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{

  symTable* test = symTable_init(NULL);

  symbol* testSymbol = NULL;

  value v42;
  v42.integer = 42;
  testSymbol = symTable_newTemp(test, INT, v42);
  assert(strcmp(testSymbol->identifier, "$temp_0")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->content.type == INT);
  assert(testSymbol->content.val.integer == 42);

  testSymbol = symTable_addConst(test, "lol");
  assert(strcmp(testSymbol->identifier, "lol")==0);
  assert(testSymbol->isConstant == true);

  value v50;
  v50.integer = 50;
  testSymbol = symTable_newTemp(test, INT, v50);
  assert(strcmp(testSymbol->identifier, "$temp_1")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->content.type == INT);
  assert(testSymbol->content.val.integer == 50);

  value v60;
  v60.integer = 60;
  testSymbol = symTable_newTemp(test, INT, v60);
  assert(strcmp(testSymbol->identifier, "$temp_2")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->content.type == INT);
  assert(testSymbol->content.val.integer == 60);

  value vStr;
  vStr.string = "LOLOLOLO";
  testSymbol = symTable_newTemp(test, STRING, vStr);
  assert(strcmp(testSymbol->identifier, "$temp_3")==0);
  assert(testSymbol->isConstant == false);
  assert(testSymbol->content.type == STRING);
  assert(strcmp(testSymbol->content.val.string, "LOLOLOLO") == 0);

  testSymbol = symTable_addConst(test, "const");
  assert(strcmp(testSymbol->identifier, "const")==0);
  assert(testSymbol->isConstant == true);

  testSymbol = symTable_addConst(test, "const");
  assert(testSymbol == NULL);

  symTable_print(test);

  symTable_free(test);

  return 0;
}
