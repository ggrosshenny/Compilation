#ifndef __QUADTOMIPS_H_
#define __QUADTOMIPS_H_

#include "astToST.h"
#include <stdio.h>
//#include "quad.h"

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
void genMIPS_genCode(char* fileName, quadList* quads , symTable table);


/**
* @brief genMIPS_genAdd Generate the MIPS code for addition with the given quad
* @param file_MIPS MIPS code file
* @param quad given quad
**/
void genMIPS_genAdd(FILE* file_MIPS/*, Quad* quad*/);


#endif
