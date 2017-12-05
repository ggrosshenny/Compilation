#include "quadToMIPS.h"



void genMIPS_error(char* message, ast* tree, symTable* st, codegen* cg, MIPS* mips)
{
  ast_free(tree);
  symTable_free(st);
  codegen_free(cg);
  genMIPS_freeMIPS(mips);
  fprintf(stderr, "%s\n", message);
  exit(1);
}


void genMIPS_freeMIPS(MIPS* mips)
{
  fclose(mips->fileMIPS);
  free(mips);
}


MIPS* genMIPS_init(char* fileName, symTable* table)
{
  MIPS* mips = calloc(1, sizeof(MIPS));
  int i = 0;
  symbol* temp = NULL;
  // Open file
  mips->fileMIPS = fopen(fileName, "w+");
  mips->headerSize = 0;

  // Add the table content to the data segment
  fprintf(mips->fileMIPS, "# MIPS code generated for compilation project\n# Grosshenny Guillaume and Wasmer Audric\n\n# Data section\n\t.data\n");
  mips->headerSize += 5;
  for(i=0; i<ST_HASHTABLE_SIZE; i++)
  {
    temp = table->table[i];
    while(temp != NULL)
    {
      if(!temp->isConstant && !temp->isFunction && !temp->isLabel)
      {
        if(temp->content.type == INT)
        {
          fprintf(mips->fileMIPS, "%s:\t.word %d\n", temp->identifier, temp->content.val.integer);
          mips->headerSize++;
        }
        else if(temp->content.type == STRING)
        {
          fprintf(mips->fileMIPS, "%s:\t.asciiz \"%s\"\n", temp->identifier, temp->content.val.string);
          mips->headerSize++;
        }

      }
      if(temp->isConstant && !temp->isFunction)
      {
        fprintf(mips->fileMIPS, "%s:\t.word 0\n", temp->identifier);
        mips->headerSize++;
      }

      temp = temp->next;
    } // While
  } // For

  // Program body
  fprintf(mips->fileMIPS, "\n\n\t.text");
  mips->headerSize+=2;
  // Library functions
    // printf
    // $(SP) : val to print
  fprintf(mips->fileMIPS, "\n\nprintf:\n");
  fprintf(mips->fileMIPS, "li\t$v0, 4\n");
  fprintf(mips->fileMIPS, "syscall\n");
  fprintf(mips->fileMIPS, "jr $ra\n");
  mips->headerSize+=6;
    // printi
    // $(SP) : val to print
  fprintf(mips->fileMIPS, "\n\nprinti:\n");
  fprintf(mips->fileMIPS, "lw\t$t0, ($sp)\n");
  fprintf(mips->fileMIPS, "move\t$a0, $t0\n");
  fprintf(mips->fileMIPS, "li\t$v0, 1\n");
  fprintf(mips->fileMIPS, "syscall\n");
  fprintf(mips->fileMIPS, "jr $ra\n");
  mips->headerSize+=8;

  // Count the two blank lines before the first function definition :
  fprintf(mips->fileMIPS, "\n\n");
  mips->headerSize += 2;
  return mips;
}


void genMIPS_genCode(char* fileName, codegen* cg , symTable* table)
{
  MIPS* mips = genMIPS_init(fileName, table);
  quad* ql = cg->code->list;

  // Get the MIPS code for all quads
  while(ql != NULL)
  {
    switch(ql->type)
    {
      // leafs
      case AST_INT     :  break;
      case AST_STR     :  break;
      case AST_ID      :  break;
      // Binary operations
      case AST_OP_ADD  :  genMIPS_genAdd(mips, ql);
                          break;
      case AST_OP_SUB  :  genMIPS_genSub(mips, ql);
                          break;
      case AST_OP_MULT :  genMIPS_genMult(mips, ql);
                          break;
      case AST_OP_DIV :   genMIPS_genDiv(mips, ql);
                          break;
      case AST_OP_AFCT:   genMIPS_genAffectation(mips, ql);
                          break;
      // Unary operations
      case AST_OP_INCR :  genMIPS_genIncr(mips, ql);
                          break;
      case AST_OP_DECR :  genMIPS_genDecr(mips, ql);
                          break;
      case AST_OP_MINUS : genMIPS_genMinus(mips, ql);
                          break;
      // Function
      case AST_FUNC_DEF : genMIPS_genFunctionDeclaration(mips, ql);
                          break;
      case AST_FUNC_ARG: ql = genMIPS_genFunctionCall(mips, ql, table, cg);
                          break;
      case AST_FUNC_CALL: ql = genMIPS_genFunctionCall(mips, ql, table, cg);
                          break;

      // Control structure
      case AST_BOOL_EQ :  genMIPS_genBranchOnEqual(mips, ql);
                          break;
      case AST_BOOL_NEQ:  genMIPS_genBranchOnNotEqual(mips, ql);
                          break;
      case AST_BOOL_GEQ:  genMIPS_genBranchOnGreaterEqual(mips, ql);
                          break;
      case AST_BOOL_LEQ:  genMIPS_genBranchOnLessEqual(mips, ql);
                          break;
      case AST_BOOL_GT :  genMIPS_genBranchOnGreaterThan(mips, ql);
                          break;
      case AST_BOOL_LT :  genMIPS_genBranchOnLessThan(mips, ql);
                          break;
      case AST_GOTO    :  genMIPS_genGOTO(mips, ql);
                          break;
      case AST_CREATE_LABEL: genMIPS_genLabel(mips, ql);
                             break;

      default         :   break;
    }
    ql = ql->next;
  }

  fprintf(mips->fileMIPS, "li $v0, 10\n");
  fprintf(mips->fileMIPS, "syscall\n\n");

  fclose(mips->fileMIPS);
  free(mips);
}


// =================
// Common operations

void genMIPS_genAffectation(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "sw\t$t0, %s\n", qd->res->identifier);
}


// =====================
// Arithmetic expression


void genMIPS_genAdd(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "add\t$t2, $t0, $t1\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genSub(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "sub\t$t2, $t0, $t1\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genMult(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "mult\t$t0, $t1\n");
  fprintf(mips->fileMIPS, "mflo\t$t2\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genDiv(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "div\t$t0, $t1\n");
  fprintf(mips->fileMIPS, "mflo\t$t2\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genIncr(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "addi\t$t1, $t0, 1\n");
  fprintf(mips->fileMIPS, "sw\t$t1, %s\n", qd->res->identifier);
}


void genMIPS_genDecr(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "li\t$t1, 1\n");
  fprintf(mips->fileMIPS, "sub\t$t2, $t0, $t1\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


void genMIPS_genMinus(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "li\t$t1, -1\n");
  fprintf(mips->fileMIPS, "mult\t$t0, $t1\n");
  fprintf(mips->fileMIPS, "mflo\t$t2\n");
  fprintf(mips->fileMIPS, "sw\t$t2, %s\n", qd->res->identifier);
}


// =========
// Functions


void genMIPS_genFunctionDeclaration(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "%s:\n", qd->arg1->identifier);
}

quad* genMIPS_genFunctionCall(MIPS* mips, quad* qd, symTable* st, codegen* cg)
{
  quad* temp = qd;
  int strSize=0;
  int argNumber = 0;
  int stackPointer = 0;
  int askedArgNumber = 0;
  args* argsList;
  symbol* func;
  symbol* arg;

  // Find function datas
  while(temp->type != AST_FUNC_CALL)
  {
    temp = temp->next;
  }
  func = symTable_lookUp(st, temp->res->identifier);

  // Verify if the number of arguments is correct and how many place we need to
  // allocate in SP
  argsList = func->content.val.arguments;
  while(argsList != NULL)
  {
    arg = symTable_lookUp(st, argsList->currentArg->identifier);
    askedArgNumber++;
    argsList = argsList->nextArg;
  }

  // Don't use the stack
  temp = qd;
  if(strcmp(func->identifier, "printf") == 0)
  {
    temp = qd;
    while(temp->type != AST_FUNC_CALL)
    {
      // Get the value of the argument stored in .data section
      fprintf(mips->fileMIPS, "la\t$a0, %s\n", temp->res->identifier);
      argNumber++;
      temp = temp->next;
    }
  }
  // Use the stack
  else
  {
    // Load all arguments in stack
    while(temp->type != AST_FUNC_CALL)
    {
      arg = temp->res;
      // Get the value of the argument
      fprintf(mips->fileMIPS, "lw\t$t0, %s\n", temp->res->identifier);
      // Allocate memory on the stack for the argument
      if(arg->content.type == STRING) // Only used for printf !
      {
        strSize = strlen(arg->content.val.string);
        // Allocate memory in stack for the asciiz
        fprintf(mips->fileMIPS, "addiu\t$sp, $sp, -%d\n", strSize+1);
        fprintf(mips->fileMIPS, "sw\t$t0, %d($sp)\n", stackPointer);
        stackPointer += strSize+1;
      }
      else if(arg->content.type == INT) // Used for all functions
      {
        // Allocate memory in stack for a word
        fprintf(mips->fileMIPS, "addiu\t$sp, $sp, -4\n");
        fprintf(mips->fileMIPS, "sw\t$t0, %d($sp)\n", stackPointer);
        stackPointer += 4;
      }

      argNumber++;

      temp = temp->next;
    }
  }

  // Arguments verification
  if((argNumber > askedArgNumber) || (argNumber < askedArgNumber))
  {
    char msg[MAX_IDENTIFIER_LENGHT + 512];
    snprintf(msg, MAX_IDENTIFIER_LENGHT+250, "Compilation error : wrong argument number in function %s (asked %d and got %d)\n", func->identifier, askedArgNumber, argNumber);
    genMIPS_error(msg, st->tree, st, cg, mips);
  }

  fprintf(mips->fileMIPS, "jal\t%s\n", func->identifier);
  // Verify if function return smth ! (add in symbol struct)
  // Add the action with $ra if necessary

  // Free memory allocated into stack
  fprintf(mips->fileMIPS, "addiu\t$sp, $sp, %d\n", stackPointer);

  return temp;
}


void genMIPS_genBranchOnEqual(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "beq\t$t0, $t1, %s\n" , qd->res->identifier);
}


void genMIPS_genBranchOnNotEqual(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "bne\t$t0, $t1, %s\n" , qd->res->identifier);}


void genMIPS_genBranchOnGreaterEqual(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "bge\t$t0, $t1, %s\n" , qd->res->identifier);}


void genMIPS_genBranchOnLessEqual(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "ble\t$t0, $t1, %s\n" , qd->res->identifier);}


void genMIPS_genBranchOnGreaterThan(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "bgt\t$t0, $t1, %s\n" , qd->res->identifier);}


void genMIPS_genBranchOnLessThan(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "lw\t$t0, %s\n", qd->arg1->identifier);
  fprintf(mips->fileMIPS, "lw\t$t1, %s\n", qd->arg2->identifier);
  fprintf(mips->fileMIPS, "blt\t$t0, $t1, %s\n" , qd->res->identifier);}


void genMIPS_genGOTO(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "j\t%s\n", qd->res->identifier);
}


void genMIPS_genLabel(MIPS* mips, quad* qd)
{
  fprintf(mips->fileMIPS, "%s:\n", qd->res->identifier);
}




//
