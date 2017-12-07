#ifndef __AST_H_
#define __AST_H_

#include "string.h"

// AST structure

// Definitions
#define  MAX_IDENTIFIER_LENGHT 42
#define  MAX_STR_LENGHT 1024


// Types definition
enum ast_type{AST_INT, AST_STR, AST_ID, // Basic types
              AST_OP_ADD, AST_OP_SUB, AST_OP_MULT, AST_OP_DIV, // Arithmetic operations
              AST_OP_INCR, AST_OP_DECR, AST_OP_MINUS, AST_OP_AFCT, AST_OP_DECL, // Variable operations
              AST_FUNC_CALL, AST_FUNC_DEF, AST_FUNC_ARG, AST_FUNC_BODY, // Functions
              AST_BOOL_NOT, AST_BOOL_EQ, AST_BOOL_NEQ, AST_BOOL_GT, AST_BOOL_GEQ, // Boolean expressions
              AST_BOOL_LT, AST_BOOL_LEQ,  //Boolean expressions
              AST_AND_TREE, AST_OR_TREE, AST_BOOL_TREE, AST_GOTO, AST_CREATE_LABEL, // Boolean structures
              AST_IF, AST_WHILE, AST_FOR,  // Control structures
              AST_TAB_DECL, AST_TAB_ACSS, AST_TAB_ELEM, AST_TAB_DIM, AST_TAB_BLCK // Table
            };

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
      struct s_ast* argument; // Have to be AST_ID or AST_INT !
      struct s_ast* nextArg;  // Have to be AST_FUNC_ARG !
    }argumentsList;
      // Body of functions
    struct
    {
      struct s_ast* instruction;     // Can be all kind of AST types
      struct s_ast* nextInstruction; // Have to be AST_FUNC_BODY !
    }instructionsList;
      // Boolean expressions
    struct
    {
      struct s_ast* boolExpr;     // Binary operation with BOOL_. type
      struct s_ast* ast_true;     // Have to be AST_BOOL_TREE
      struct s_ast* ast_false;    // Have to be AST_BOOL_TREE
      struct s_ast* forVarInit;   // statement
      struct s_ast* forVarUpdate; // Must be either an affectation, an incrementation or a decrementation
    }boolean;
      // Table declaration structure
    struct
    {
      struct s_ast* identifier;   // Have to be AST_STR
      struct s_ast* dimensions;   // Have to be AST_TAB_DIM
      struct s_ast* elements;     // Have to be AST_TAB_BLCK (is NULL if there is no tab affectation)
    }tableDeclaration;
      // table dimensions with their sizes
    struct
    {
      struct s_ast* val;      // Have to be AST_INT
      struct s_ast* nextDim;  // Have to be AST_TAB_DIM
    }tableDimensionsList;
      // Table elements
    struct
    {
      struct s_ast* currentElem;  // Have to be AST_NUM or AST_TAB_DECL
      struct s_ast* nextElem;     // Have to be AST_TAB_ELEM
    }tableElementsList;
    struct
    {
      struct s_ast* elements;  // Have to be AST_TAB_ELEM
      struct s_ast* nextBlock; // Have to be AST_TAB_BLCK
    }tableElementsBlock;
    struct
    {
      struct s_ast* identifier;   // Have to be AST_ID
      struct s_ast* indices;      // Have to be AST_TAB_DIM
    }tableAccess;
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
 * @brief placeGoto   Replace every NULL pointer in boolTree's members with GOTO ASTs
 *                    pointing to true0 or false0
 * @param boolTree    AST to be processed
 * @param true0       Instructions block to be pointed by ast_true members of the AST
 * @param false0      Instructions block to be pointed by ast_false members of the AST
 */
void placeGoto(ast* boolTree, ast* true0, ast* false0);

/**
 * @brief ast_new_controlStructure Create a list of boolean conditions
 * @param type  Type of the boolean global expression
 * @param conditionsList0  Current boolean expression in the list
 * @param true0 Actions to be done if the control structure's result is true
 * @param false0 Actions to be done if the control structure's result is false
 * @param varInit0 In case the control structure is a for : variable initialization
 * @param varUpdate0 In cas the control structure is a for : variable update at each iteration
 */
ast* ast_new_controlStructure(enum ast_type type, ast* conditionsList0, ast* true0, ast* false0, ast* varInit0, ast* varUpdate0);

/**
 * @brief ast_new_boolExpr Create a new boolean expression
 * @param boolExpr0 Boolean expression
 * @param ast_true0 Actions to be done if the expression is true
 * @param ast_false0 Actions to be done if the expression is false
 */
ast* ast_new_boolExpr(ast* boolExpr0, ast* ast_true0, ast* ast_false0);

  // Table


/**
 * @brief ast_new_tabDeclaration Create a new table declaration
 * @param identifier0 identifier AST of the table
 * @param dimensions0 dimensions AST of the table
 * @param elements0 elements of the table
 */
ast* ast_new_tabDeclaration(ast* identifier0, ast* dimensions0, ast* elements0);


/**
 * @brief ast_new_tableAccess Create a new table access
 * @param identifier0 identifier AST of the table
 * @param indices0 indices AST of the table
 */
ast* ast_new_tableAccess(ast* identifier0, ast* indices0);


/**
 * @brief ast_new_tableAccess Create a new table access
 * @param valeur AST of the valeur of the dimension
 */
ast* ast_new_tabDimension(ast* valeur);


/**
 * @brief ast_new_tabElements Create a new element
 * @param identifier0 identifier AST of the table
 */
ast* ast_new_tabElements(ast* element);


/**
 * @brief ast_new_tabElements Create a new element
 * @param block elements list to encapsulate
 */
ast* ast_new_tableElementsBlock(ast* block);


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
