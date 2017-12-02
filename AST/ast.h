#ifndef __AST_H_
#define __AST_H_

#include "string.h"

// AST structure

// Definitions
#define  MAX_IDENTIFIER_LENGHT 42
#define  MAX_STR_LENGHT 1024


// Types definition
enum ast_type{AST_INT, AST_STR, AST_OP_ADD, AST_OP_SUB, AST_OP_MULT, AST_OP_DIV,
              AST_OP_INCR, AST_OP_DECR, AST_OP_MINUS, AST_OP_AFCT, AST_OP_DECL,
              AST_FUNC_CALL, AST_FUNC_DEF, AST_FUNC_ARG, AST_BOOL_NOT, AST_BOOL_EQ,
              AST_BOOL_NEQ, AST_BOOL_GT, AST_BOOL_GEQ, AST_BOOL_LT, AST_BOOL_LEQ,
              AST_AND_TREE, AST_OR_TREE, AST_BOOL_TREE, AST_GOTO, AST_IF, AST_FUNC_BODY,
              AST_ID};

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
      struct s_ast* identifier; // Have to be AST_ID
      struct s_ast* arguments;  // Have to be AST_FUNC_ARG
      struct s_ast* body;       // Have to be AST_FUNC_BODY
    }function;
      // Arguments for functions
    struct
    {
      struct s_ast* prevArg; // Have to be AST_FUNC_ARG !
      struct s_ast* argument; // Have to be AST_ID or AST_INT !
      struct s_ast* nextArg; // Have to be AST_FUNC_ARG !
    }argumentsList;
      // Body of functions
    struct
    {
      struct s_ast* prevInstruction; // Have to be AST_FUNC_BODY !
      struct s_ast* instruction;     // Can be all kind of AST types
      struct s_ast* nextInstruction; // Have to be AST_FUNC_BODY !
    }instructionsList;
      // Boolean expressions
    struct
    {
      struct s_ast* boolExpr;     // Binary operation with BOOL_. type
      struct s_ast* ast_true;     // Have to be AST_BOOL_TREE
      struct s_ast* ast_false;    // Have to be AST_BOOL_TREE
    }boolean;
      // Number
    int number;
      // string
    char* string;
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


/**
* @brief ast_concat Concat two ast
* @param right right ast
* @param left left ast
*/
ast* ast_concat(ast* right, ast* left);


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


  // Instructions list
/**
* @brief ast_new_Instruction Create a new instruction node in the given ast
* @param instruction0 instruction to encapsulate
*/
ast* ast_new_Instruction(ast* instruction0);


/**
* @brief ast_new_argument Create a new argument node in the given ast
* @param argument 0 argument to encapsulate
*/
ast* ast_new_argument(ast* argument0);


  // Boolean expressions
/**
 * @brief ast_new_conditionsList Create a list of boolean conditions
 * @param type  Type of the boolean global expression
 * @param condition0  Current boolean expression in the list
 * @param nextCondition0 Next boolean expression in the list
 */
ast* ast_new_conditionsList(enum ast_type type, ast* condition0, ast* nextCondition0);

/**
 * @brief ast_new_boolExpr Create a new boolean expression
 * @param boolExpr0 Boolean expression
 * @param ast_true0 Actions to be done if the expression is true
 * @param ast_false0 Actions to be done if the expression is false
 */
ast* ast_new_boolExpr(ast* boolExpr0, ast* ast_true0, ast* ast_false0);

  // Variables and integers

/**
* @brief ast_new_number Create a new number in the given ast
* @param value
*/
ast* ast_new_number(int value);


/**
* @brief ast_new_string Create a new string in the given ast
* @param str
*/
ast* ast_new_string(char* str);


/**
* @brief ast_new_identifier Create a new identifier in the given ast
* @param id
*/
ast* ast_new_identifier(char* id);


#endif
