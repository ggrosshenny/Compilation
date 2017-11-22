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
  while(temp != NULL)
  {
      if(temp->isConstant)
      {
        printf("\t%s\tNONE\n", temp->identifier);
      }
      else
      {
        printf("\t%s\t%d\n", temp->identifier, temp->value);
      }
      temp = temp->next;
  }
}


void symTable_print(symTable* table)
{
  int i = 0;
  printf("Symbol table :\n");
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    symbol_printList(table->table[i]);
  }
}


// ==============
// Main functions


symTable* symTable_init()
{
  symTable* newTable = malloc(sizeof(symTable));
  for(int i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    newTable->table[i] = NULL;
  }
  newTable->nb_temp = 0;
  newTable->nb_VarInStack = 0;
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
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
      current = table->table[i];
      while(current != NULL)
      {
        temp = current;
        current = current->next;
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
  symbol0->stackPosition = -1;
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


symbol* symTable_newTemp(symTable* table, int value)
{
  char* tempName = malloc(ST_MAX_TEMPIDENTIFIER_LENGTH * sizeof(char));
  // Create the temp name
  // temp variable begin with $temp_
  snprintf(tempName, ST_MAX_TEMPIDENTIFIER_LENGTH, "$temp_%d", table->nb_temp);
  table->nb_temp++;

  // Create the new temp variable
  symbol* newTemp = malloc(sizeof(symbol));
  newTemp->identifier = tempName;
  newTemp->isConstant = false;
  newTemp->value = value;
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
  // Create the new temp variable
  symTable* temp = NULL;
  symbol* newConst = malloc(sizeof(symbol));
  newConst->identifier = strndup(constName, ST_MAX_IDENTIFIER_LENGTH);
  newConst->isConstant = true;
  newConst->value = 0;
  temp = symTable_add(table, newConst);
  if(temp == NULL)
  {
      return NULL;
  }
  return newConst;
}
