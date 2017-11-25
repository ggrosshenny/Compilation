#include "quadToMIPS.h"


FILE* genMIPS_init(char* fileName, symTable* table)
{
  int i = 0;
  symbol* temp = NULL;
  // Open file
  FILE* fileMIPS;
  fileMIPS = fopen(fileName, "w+");

  // Add the table content to the data segment
  fprintf(fileMIPS, "# MIPS code generated for compilation project\n# Grosshenny Guillaume and Wasmer Audric\n\n# Data section\n\t.data\n");
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

  fprintf(fileMIPS, "\n\n\t.text\n\nmain:\n\n");

  return fileMIPS;
}


void genMIPS_genCode(char* fileName, quadList* quads , symTable* table)
{
  FILE* fileMIPS = genMIPS_init(fileName, table);
  quad* ql = quads->list;

  // Get the MIPS code for all quads
  while(ql != NULL)
  {
    switch(ql->type)
    {
      // leafs
      case AST_INT     :  break;
      case AST_ID      :  break;
      // Binary operations
      case AST_OP_ADD  :  genMIPS_genAdd(fileMIPS, ql);
                          break;
      case AST_OP_SUB  :  genMIPS_genSub(fileMIPS, ql);
                          break;
      case AST_OP_MULT :  genMIPS_genMult(fileMIPS, ql);
                          break;
      case AST_OP_DIV :   genMIPS_genDiv(fileMIPS, ql);
                          break;
      // Unary operations
      case AST_OP_INCR :  genMIPS_genIncr(fileMIPS, ql);
                          break;
      case AST_OP_DECR :  genMIPS_genDecr(fileMIPS, ql);
                          break;
      case AST_OP_MINUS : genMIPS_genMinus(fileMIPS, ql);
                          break;
      // Function
      case AST_FUNC_DEF :
                          break;
      case AST_FUNC_CALL:
                          break;

      default         :   break;
    }
    ql = ql->next;
  }
  fprintf(fileMIPS, "jr $ra\n\n");
  fclose(fileMIPS);
}


void genMIPS_genAdd(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(fileMIPS, "add\t$t2, $t0, $t1\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genSub(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(fileMIPS, "sub\t$t2, $t0, $t1\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genMult(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(fileMIPS, "mult\t$t0, $t1\n");
  fprintf(fileMIPS, "mflo\t$t2\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genDiv(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(fileMIPS, "div\t$t0, $t1\n");
  fprintf(fileMIPS, "mflo\t$t2\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genIncr(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "addi\t$t1, $t0, 1\n");
  fprintf(fileMIPS, "sw\t$t1, %s\n", qd->res->identifier);
}


void genMIPS_genDecr(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "li\t$t1, 1\n");
  fprintf(fileMIPS, "sub\t$t2, $t0, $t1\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genMinus(FILE* fileMIPS, quad* qd)
{
  fprintf(fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(fileMIPS, "li\t$t1, -1\n");
  fprintf(fileMIPS, "mult\t$t0, $t1\n");
  fprintf(fileMIPS, "mflo\t$t2\n");
  fprintf(fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}














//
