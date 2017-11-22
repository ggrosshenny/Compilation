#ifndef __AST_H_
#define __AST_H_

#include "string.h"

// AST structure

// Definitions
#define  MAX_IDENTIFIER_LENGHT 42


// Types definition
enum ast_type{AST_INT, AST_OP_ADD, AST_OP_SUB, AST_OP_MULT, AST_OP_DIV,
              AST_OP_INCR, AST_OP_DECR, AST_OP_MINUS, AST_OP_AFCT, AST_FUNC_CALL, 
              AST_FUNC_DEF, AST_ID};

// Ast definition
typedef struct s_ast
{
  // Type of the node
  enum ast_type type;
  // Structure of the node
  union
  {
      // Operation
    struct
    {
      struct s_ast* left;
      struct s_ast* right;
    }operation;
      // Function call and definition
    struct
    {
      struct s_ast* identifier;
      struct s_ast* arguments;
      struct s_ast* body;
    }function;
      // Number
    int number;
      // Identifier
    char* identifier;
  }component ;
} ast;


// Auxiliary functions

/**
* @brief print_ast Print the given ast
* @param tree
* @param indent
*/
void print_ast(ast* tree, int indent);


// Memory release


/**
* @brief ast_free free the AST
* @param tree
*/
void ast_free(ast* tree);


// Memory allocation

  // Operations
/**
* @brief ast_new_binaryOperation Create a new binary operation node in the given ast
* @param type type of the node
* @param left0 left child
* @param right0 right child
*/
ast* ast_new_binaryOperation(enum ast_type type, ast* left0, ast* right0);


/**
* @brief ast_new_operation Create a new unary operation node in the given ast
* @param type type of the node
* @param operand child
*/
ast* ast_new_unaryOperation(enum ast_type type, ast* operand);


  // Functions
  // Only main()
/**
* @brief ast_new_functionDefinition Create a new function definition node in the given ast
* @param type type of the node
* @param id identifier of the function
* @param arguments arguments of the function
* @param functionBody body of the function
*/
ast* ast_new_functionDefinition(ast* id, ast* arguments, ast* functionBody);


  // Only main(), printf(str), printi(int) availiable
/**
* @brief ast_new_functionDefinition Create a new function call node in the given ast
* @param type type of the node
* @param id identifier of the function
* @param arguments arguments of the function
*/
ast* ast_new_functionCall(ast* id, ast* arguments);


  // Variables and integers

/**
* @brief ast_new_number Create a new number in the given ast
* @param value
*/
ast* ast_new_number(int value);


/**
* @brief ast_new_identifier Create a new identifier in the given ast
* @param id
*/
ast* ast_new_identifier(char* id);



#endif
