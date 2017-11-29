#ifndef __QUADTOMIPS_H_
#define __QUADTOMIPS_H_

#include "astToST.h"
#include <stdio.h>
#include "quad.h"

// Auxiliary functions


// Mains functions


/**
* @brief genMIPS_init initialize the MIPS code generation. Open the file for MIPS
* code and store the symbol table in the .data section of the MIPS code
* @param fileName name of the MIPS file
* @param table symbol table
*/
FILE* genMIPS_init(char* fileName, symTable* table);


/**
* @brief genMIPS_genCode generate the code for each quads in the list of quads.
* @param fileName name of the MIPS file
* @param quads list of quads
* @param table symbol table
*/
void genMIPS_genCode(char* fileName, quadList* quads , symTable* table);


// =================
// Common operations


/**
* @brief genMIPS_genAffectation Generate the MIPS code for the affectation with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genAffectation(FILE* fileMIPS, quad* qd);


// ============================
// Binary arithmetic operations


/**
* @brief genMIPS_genAdd Generate the MIPS code for the addition with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genAdd(FILE* fileMIPS, quad* qd);


/**
* @brief genMIPS_genSub Generate the MIPS code for the substraction with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genSub(FILE* fileMIPS, quad* qd);


/**
* @brief genMIPS_genMult Generate the MIPS code for the multiplication with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genMult(FILE* fileMIPS, quad* qd);


/**
* @brief genMIPS_genDiv Generate the MIPS code for the division with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genDiv(FILE* fileMIPS, quad* qd);


// ============================
// Unary arithmetic operations


/**
* @brief genMIPS_genIncr Generate the MIPS code for the incrementation with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genIncr(FILE* fileMIPS, quad* qd);


/**
* @brief genMIPS_genDecr Generate the MIPS code for the decrementation with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genDecr(FILE* fileMIPS, quad* qd);


/**
* @brief genMIPS_genMinus Generate the MIPS code for the minus with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genMinus(FILE* fileMIPS, quad* qd);


// ====================
// Functions operations


/**
* @brief genMIPS_genFunctionDeclaration Generate the MIPS code for the function declaration with the given quad
* @param file_MIPS MIPS code file
* @param qd given quad
**/
void genMIPS_genFunctionDeclaration(FILE* fileMIPS, quad* qd);

#endif
