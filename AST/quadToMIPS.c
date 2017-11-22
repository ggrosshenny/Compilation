#include "quadToMIPS.h"


FILE* genMIPS_init(char* fileName, symTable* table)
{
  int i = 0;
  symbol* temp = NULL;
  // Open file
  FILE* fileMIPS;
  fileMIPS = fopen(fileName, "w+");

  // Add the table content to the data segment
  fprintf(fileMIPS, "# MIPS code generated for compilation project\n# Grosshenny Guillaume and Wasmer Audric\n\n# Data section\n.data\n");
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    temp = table->table[i];
    while(temp != NULL)
    {
      if(!temp->isConstant)
      {
        fprintf(fileMIPS, "%s:\t.word %d\n", temp->identifier, temp->value);
      }
      if(temp->isConstant)
      {
        fprintf(fileMIPS, "%s:\t.word 0\n", temp->identifier);
      }

      temp = temp->next;
    } // While
  } // For

  fprintf(fileMIPS, "\n\n.text\n");

  return fileMIPS;
}
