#ifndef __QUADTOMIPS_H_
#define __QUADTOMIPS_H_

#include "astToST.h"
#include <stdio.h>
#include "quad.h"


// Structures
typedef struct s_MIPS
{
  FILE* fileMIPS;
  int headerSize;
}MIPS;


// Auxiliary functions


/**
* @brief genMIPS_error free all AST, symbol table, quads list and close MIPS code file
* @param message Error message
* @param tree AST
* @param st symbol table
* @param cg codegen
* @param mips MIPS object
*/
void genMIPS_error(char* message, ast* tree, symTable* st, codegen* cg, MIPS* mips);

// Mains functions


/**
* @brief genMIPS_init initialize the MIPS code generation. Open the file for MIPS
* code and store the symbol table in the .data section of the MIPS code
* @param fileName name of the MIPS file
* @param table symbol table
*/
MIPS* genMIPS_init(char* fileName, symTable* table);


/**
* @brief genMIPS_freeMIPS free the memory allocated for the MIPS object
* @param mips
**/
void genMIPS_freeMIPS(MIPS* mips);


/**
* @brief genMIPS_genCode generate the code for each quads in the list of quads.
* @param fileName name of the MIPS file
* @param cg codegen
* @param table symbol table
*/
void genMIPS_genCode(char* fileName, codegen* cg, symTable* table);


// =================
// Common operations


/**
* @brief genMIPS_genAffectation Generate the MIPS code for the affectation with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genAffectation(MIPS* mips, quad* qd);


// ============================
// Binary arithmetic operations


/**
* @brief genMIPS_genAdd Generate the MIPS code for the addition with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genAdd(MIPS* mips, quad* qd);


/**
* @brief genMIPS_genSub Generate the MIPS code for the substraction with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genSub(MIPS* mips, quad* qd);


/**
* @brief genMIPS_genMult Generate the MIPS code for the multiplication with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genMult(MIPS* mips, quad* qd);


/**
* @brief genMIPS_genDiv Generate the MIPS code for the division with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genDiv(MIPS* mips, quad* qd);


// ============================
// Unary arithmetic operations


/**
* @brief genMIPS_genIncr Generate the MIPS code for the incrementation with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genIncr(MIPS* mips, quad* qd);


/**
* @brief genMIPS_genDecr Generate the MIPS code for the decrementation with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genDecr(MIPS* mips, quad* qd);


/**
* @brief genMIPS_genMinus Generate the MIPS code for the minus with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genMinus(MIPS* mips, quad* qd);


// ====================
// Functions operations


/**
* @brief genMIPS_genFunctionDeclaration Generate the MIPS code for the function declaration with the given quad
* @param mips MIPS code object
* @param qd given quad
**/
void genMIPS_genFunctionDeclaration(MIPS* mips, quad* qd);


/**
* @brief genMIPS_functionCall Generate the MIPS code for the function call with the given quad
* @param mips MIPS code object
* @param qd given quad
* @return
**/
quad* genMIPS_genFunctionCall(MIPS* mips, quad* qd, symTable* st, codegen* cg);

#endif
