#ifndef __SYMBOLTABLE_H_
#define __SYMBOLETABLE_H_

#include <stdbool.h>

#define ST_MAX_IDENTIFIER_LENGTH 42
#define ST_MAX_TEMPIDENTIFIER_LENGTH 48
#define ST_HASHTABLE_SIZE 4096

// Dans la TDS : Number, Variable

// ==========
// Structures

typedef struct s_symbol
{
  char* identifier;
  bool isConstant;
  int value;
  struct s_symbol* next;
} symbol;

typedef struct s_symTable
{
  symbol* table[ST_HASHTABLE_SIZE];
  int nb_temp;
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
symTable* symTable_init();


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
 * @brief symTable_newTemp create a new constante in the table
 * @param table symbol table
 * @param tempName name of the constante
 * @param value value of constante
 **/
symbol* symTable_addConst(symTable* table, char* constName);


/**
 * @brief symTable_lookUp look in the table if the element with the given name is already registerd or not
 * @param table symbol table
 * @param name name of the element
 **/
symbol* symTable_lookUp(symTable* table, char* name);

#endif
