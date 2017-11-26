#ifndef __SYMBOLTABLE_H_
#define __SYMBOLETABLE_H_

#include <stdbool.h>
#include "ast.h"

#define ST_MAX_IDENTIFIER_LENGTH 42
#define ST_MAX_TEMPIDENTIFIER_LENGTH 48
#define ST_HASHTABLE_SIZE 4096

// Dans la TDS : Number, Variable

// ==========
// Structures

typedef struct s_symbol
{
  // Identifier of the symbol
  char* identifier;
  // Bool to know if the symbol is a constant or a temporary variable
  bool isConstant;
  // Bool to know if the symbol is a function
  bool isFunction;
  // Value of the temporary
  int value;
  // Position of the symbol in stack (used for translation from quad to MIPS)
  int stackPosition;
  // Next symbol in the list
  struct s_symbol* next;
} symbol;

typedef struct s_symTable
{
  // Hash table
  symbol* table[ST_HASHTABLE_SIZE];
  // Number of temporary variables in the table
  int nb_temp;
  // Number of variable in the stack (used for translation from quad to MIPS)
  int nb_VarInStack;
  // Pointer to ast tree for error handling
  ast* tree;
} symTable;


// =================
// Auxiliary methods


/**
* @brief hash return the hash code of the given key
* @param key key to hash
*/
int hash(char* key);


/**
* @brief symbol_printList print the given list of symbols
* @param symbolList list of symbols
*/
void symbol_printList(symbol* symbolList);


/**
* @brief symTable_print print the symbol table
* @param table table to print
*/
void symTable_print(symTable* table);


// ==============
// Main functions


/**
* @brief symTable_init initialize a new symbol table
*/
symTable* symTable_init(ast* tree0);


/**
 * @brief symbol_free free the memory allocated for the given symbol
 * @param symbol0
 **/
void symbol_free(symbol* symbol0);



/**
 * @brief symTable_free free the memory allocated for the given symbol table
 * @param table
 **/
void symTable_free(symTable* table);


/**
 * @brief symTable_add Add the symbol in the symTable
 * @param table symbol table
 * @param symbol symbol to add
 **/
symTable* symTable_add(symTable* table, symbol* symbol0);


/**
 * @brief symTable_newTemp create a new temporary variable in the table
 * @param table symbol table
 * @param tempName name of the temporary variable
 * @param value value of the temporary variable
 **/
symbol* symTable_newTemp(symTable* table, int value);


/**
 * @brief symTable_addConst create a new constante in the table
 * @param table symbol table
 * @param tempName name of the constante
 **/
symbol* symTable_addConst(symTable* table, char* constName);


/**
 * @brief symTable_addFunc create a new function in the table
 * @param table symbol table
 * @param funcName name of the function
 **/
symbol* symTable_addFunc(symTable* table, char* funcName);


/**
 * @brief symTable_lookUp look in the table if the element with the given name is already registerd or not
 * @param table symbol table
 * @param name name of the element
 **/
symbol* symTable_lookUp(symTable* table, char* name);

#endif
