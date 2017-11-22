
#ifndef QUAD_H
#define QUAD_H

#include <stdlib.h>
#include <stdio.h>

//#include "../AST/ast.h"
//#include "../AST/symbolTable.h"
#include "../AST/astToST.h"

typedef struct s_quad {
  enum ast_type type; // Operator
  symbol* arg1; // First argument
  symbol* arg2; // Second argument
  symbol* res;  // Result
  struct s_quad* next; // Next quad in the NULL-terminated linked list
} quad;

typedef struct s_quadList{
  quad* list;
  int nextQuad;
  quad* last;
} quadList;

typedef struct s_codegen {
  symbol* result;
  quadList* code;
} codegen;

/**
 * @brief quad_init Creates a new quad pointer & returns it
 **/
quad* quad_init();

quadList* quadList_init();

void init_existing_quad(quad* newQuad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result);
/**
 * @brief codegen_init Creates a new codegen pointer & returns it
 **/
codegen* codegen_init();

/**
 * @brief quad_free Frees the memory allocated to the quad passed as parameter
 * @param quad Quad to be freed
 **/
void quad_free(quad* quad);

void concat(quadList* q1, quadList* q2);

/**
 * @brief quad_add Add a new intialized quad in the last position of the chain list
 *        of quads of the parameter quad's member "next"
 * @param quad Quad in which the newly initialized quad will be added as a chained list member
 * @param op Operator of the 3-adresses expression represented by the new quad
 * @param arg_1 First argument of the new quad's expression
 * @param arg_2 Second argument of the new quad's expression
 * @param result Adress of the new quad's expression's result
 **/
void quad_add(quadList* quad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result);


/**
* @brief codegen_ast  Generate quads through recursive AST analysis
* @param ast Tree to analyze
* @param symbol_table
**/
codegen* codegen_ast(ast* ast, symTable* symbol_table);


#endif
