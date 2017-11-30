
#ifndef QUAD_H
#define QUAD_H

#include <stdlib.h>
#include <stdio.h>

//#include "../AST/ast.h"
//#include "../AST/symbolTable.h"
#include "../AST/astToST.h"

// Quad definition
typedef struct s_quad {
  enum ast_type type; // Operator
  symbol* arg1; // First argument
  symbol* arg2; // Second argument
  symbol* res;  // Result
  int index;    // Index of the quad
  struct s_quad* next; // Next quad in the NULL-terminated linked list
} quad;

// Quads list definition
typedef struct s_quadList{
  quad* list;
  quad* last;
  int indexNextQuad;
} quadList;

// Code generation structure definition
typedef struct s_codegen {
  symbol* result;
  quadList* code;
} codegen;

/**
 * @brief quad_init Creates a new quad pointer & returns it
 **/
quad* quad_init();

/**
 * @brief quadList_init Creates a new quadList pointer & returns it
 **/
quadList* quadList_init();

/**
 * @Brief Initialize members of an already created quad with values passed as
 * arguments to this function
 * @param newQuad Quad to be initialized
 * @param op Operator of the 3-adresses expression represented by the new quad
 * @param arg_1 First argument of the new quad's expression
 * @param arg_2 Second argument of the new quad's expression
 * @param result Adress of the new quad's expression's result
 **/
quad* init_existing_quad(enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result, int quadIndex);

void quadList_print(quadList* ql);


/**
 * @brief codegen_init Creates a new codegen pointer & returns it
 **/
codegen* codegen_init();

/**
 * @brief quad_free Frees the memory allocated to the quad passed as parameter
 * @param quad Quad to be freed
 **/
void quad_free(quad* quad);

/**
 * @brief quadList_free_keepList Free the quadList object without freeing the actual
 * list of quads referenced by the object.
 * @param ql QuadList object to be freed
 **/
void quadList_free_keepList(quadList* ql);

/**
 * @brief quadList_free Free the entier quads list
 * @param ql quadList object to be freed
 **/
void quadList_free(quadList* ql);

/**
 * @brief codegen_free Free the codegen object
 * @param cg codegen object to be freed
 **/
void codegen_free(codegen* cg);

/**
 * @brief concat Concat two lists of quads together
 * @param q1 First list to be concataned
 * @param q2 Second list, concatened to q1
 **/
quadList* concat(quadList* q1, quadList* q2);

/**
 * @brief quad_add Add a new intialized quad in the last position of the chain list
 *        of quads of the parameter quad's member "next"
 * @param quad Quad in which the newly initialized quad will be added as a chained list member
 * @param type Type of the AST node being transformed to a 3-adresses expression
 * @param arg_1 First argument of the new quad's expression
 * @param arg_2 Second argument of the new quad's expression
 * @param result Adress of the new quad's expression's result
 **/
void quad_add(quadList* quad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result);


/**
 * @brief codegen_ast_operations Auxiliary function for codegen_ast
 * @param cg Codegen instance
 * @param type Type of the AST node being transformed to a 3-adresses expression
 * @param left Left member of the codegen instance being analyzed
 * @param right Right member of the codegen instance being analyzed
 * @param symbol_table
 **/
void codegen_ast_operations(codegen* cg, enum ast_type type, codegen* left, codegen* right, symTable* symbol_table);


/**
 * @brief codegen_ast_affectation Auxiliary function for codegen_ast
 * @param cg Codegen instance
 * @param type Type of the AST node being transformed to a 3-adresses expression
 * @param left Left member of the codegen instance being analyzed
 * @param right Right member of the codegen instance being analyzed
 * @param symbol_table
 **/
void codegen_ast_affectation(codegen* cg, enum ast_type type, codegen* left, codegen* right);


/**
 * @brief codegen_ast_functionBody Auxiliary function for codegen_ast that generate the quads for the function body
 * @param cg Codegen instance
 * @param instruction current instruction to convert to quad
 * @param nextInstruction next instruction to convert to quad
 * @param ast AST
 * @param symbol_table
 **/
void codegen_ast_functionBody(codegen* cg, codegen* instruction, codegen* nextInstruction, ast* ast, symTable* symbol_table);



/**
 * @brief codegen_ast_functionArguments Auxiliary function for codegen_ast that generate the quads for the function arguments (when the function is called)
 * @param cg Codegen instance
 * @param argument current argument to convert to quad
 * @param nextArgument next argument to convert to quad
 * @param ast AST
 * @param symbol_table
 **/
void codegen_ast_functionArguments(codegen* cg, codegen* argument, codegen* nextArgument, ast* ast, symTable* symbol_table);


/**
 * @brief codegen_ast_functionCall Auxiliary function for codegen_ast that generate the quads for the function call
 * @param cg Codegen instance
 * @param arguments arguments to convert to quad
 * @param identifier name of the function to call
 **/
void codegen_ast_functionCall(codegen* cg, codegen* arguments, codegen* identifier, ast* ast, symTable* symbol_table);


/**
* @brief codegen_ast  Generate quads through recursive AST analysis
* @param ast Tree to analyze
* @param symbol_table
**/
codegen* codegen_ast(codegen* cg, ast* ast, symTable* symbol_table);


#endif
