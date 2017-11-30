#include "symbolTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// ===================
// Auxiliary functions

int hash(char* key)
{
  unsigned int h = 0;

  // Hash
	for(unsigned int i = 0 ; i < strlen(key) ; i++)
	{
    // cyclic shift of bytes
		h = (h << 5) | (h >> 27);
		h += (int) key[i];
	}
  h = h % ST_HASHTABLE_SIZE;
  return h;
}


void symbol_printList(symbol* symbolList)
{
  symbol* temp = symbolList;
  args* tempArg = NULL;
  while(temp != NULL)
  {
      if(temp->isConstant)
      {
        printf("\t%s\tNONE\n", temp->identifier);
      }
      else if(temp->isFunction)
      {
        printf("\t%s\tfunc", temp->identifier);
        if(!temp->isVoidFunction)
        {
          tempArg = temp->content.val.arguments;
          while(tempArg != NULL)
          {
            printf(" - %s", tempArg->currentArg->identifier);
            tempArg = tempArg->nextArg;
          }
        }
        printf("\n");
      }
      else
      {
        if(temp->content.type == INT)
        {
          printf("\t%s\t%d\n", temp->identifier, temp->content.val.integer);
        }
        if(temp->content.type == STRING)
        {
          printf("\t%s\t%s\n", temp->identifier, temp->content.val.string);
        }

      }
      temp = temp->next;
  }
}


void symTable_print(symTable* table)
{
  int i = 0;
  printf("Symbol table :\n");
  printf("\tid\ttype\tvalue (or arguments list)\n");
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    symbol_printList(table->table[i]);
  }
}


// ==============
// Main functions


symTable* symTable_init(ast* tree0)
{
  symTable* newTable = calloc(1, sizeof(symTable));
  for(int i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    newTable->table[i] = NULL;
  }
  newTable->nb_temp = 0;
  newTable->tree = tree0;

  // symTable_addFunc(newTable, "printf");
  // symTable_addFunc(newTable, "printi");

  return newTable;
}


void symbol_free(symbol* symbol0)
{
  free(symbol0->identifier);
  free(symbol0);
}


void symTable_free(symTable* table)
{
  int i=0;
  symbol* current = NULL;
  symbol* temp = NULL;
  args* arg = NULL;
  args* tempArg = NULL;
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
      current = table->table[i];
      while(current != NULL)
      {
        temp = current;
        current = current->next;
        if(temp->isFunction)
        {
          arg = temp->content.val.arguments;
          while(arg != NULL)
          {
            tempArg = arg;
            arg = arg->nextArg;
            free(tempArg);
          }
        }
        symbol_free(temp);
      }
  }
  free(table);
}


symbol* symTable_lookUp(symTable* table, char* name)
{
  int k_hash = hash(name);
  symbol* answ = NULL;

  while(table->table[k_hash] != NULL)
  {
    answ = table->table[k_hash];
    if(strcmp(answ->identifier, name)==0)
    {
      return answ;
    }
  }
  return answ;
}


symTable* symTable_add(symTable* table, symbol* symbol0)
{
  int k_hash = hash(symbol0->identifier);
  symbol* temp;
  // If the liste is empty
  if(table->table[k_hash] == NULL)
  {
    table->table[k_hash] = symbol0;
  }
  // If the symbol was not already stored
  else if(symTable_lookUp(table, symbol0->identifier) == NULL)
  {
    // Add at the end of the list
    temp = table->table[k_hash];
    while(temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = symbol0;
  }
  else
  {
    symbol_free(symbol0);
    return NULL;
  }
  return table;
}


symbol* symTable_newTemp(symTable* table, enum value_type val_type, value val0)
{
  char* tempName = calloc(ST_MAX_TEMPIDENTIFIER_LENGTH+1, sizeof(char));
  // Create the temp name
  // temp variable begin with $temp_
  snprintf(tempName, ST_MAX_TEMPIDENTIFIER_LENGTH+1, "$temp_%d", table->nb_temp);
  table->nb_temp++;

  // Create the new temp variable
  symbol* newTemp = calloc(1, sizeof(symbol));
  newTemp->identifier = tempName;
  newTemp->isConstant = false;
  newTemp->isFunction = false;
  newTemp->content.type = val_type;
  newTemp->content.val = val0;
  symTable* test = symTable_add(table, newTemp);
  // Should never happen
  if(test == NULL)
  {
    table->nb_temp--;
    return NULL;
  }
  return newTemp;
}


symbol* symTable_addConst(symTable* table, char* constName)
{
  // Create the new constant
  symTable* temp = NULL;
  symbol* newConst = calloc(1, sizeof(symbol));
  newConst->identifier = strndup(constName, ST_MAX_IDENTIFIER_LENGTH);
  newConst->isConstant = true;
  newConst->isFunction = false;
  temp = symTable_add(table, newConst);
  if(temp == NULL)
  {
      return NULL;
  }
  return newConst;
}


symbol* symTable_addFunc(symTable* table, char* funcName)
{
  // Create the new constant
  symTable* temp = NULL;
  symbol* newFunc = calloc(1, sizeof(symbol));
  newFunc->identifier = strndup(funcName, ST_MAX_IDENTIFIER_LENGTH);
  newFunc->isConstant = false;
  newFunc->isFunction = true;
  temp = symTable_add(table, newFunc);
  if(temp == NULL)
  {
      return NULL;
  }
  return newFunc;
}
