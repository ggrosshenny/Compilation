#ifndef __ASTTOST_H_
#define __ASTTOST_H_

#include "ast.h"
#include "symbolTable.h"


// Auxiliary functions


/**
 * @brief genSymTable_error free allocated memory and stop the program with the
 * given error message
 * @param message error message to display
 * @param tree AST to delete
 * @param st symbol table to delete
 **/
void genSymTable_error(char* message, ast* tree, symTable* st);


// Main functions


/**
 * @brief genSymTable_init generate the symbol table from the given AST with
 * symbol table initialization and add functions printi and printf to symbol table
 * @param tree AST
 **/
symTable* genSymTable_init(ast* tree);


/**
 * @brief genSymTable_ast generate the symbol table from the given AST
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_ast(ast* tree, symTable* st);


/**
 * @brief genSymTable_binaryOperation generate the symbol table from the given AST for a binary operation
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_binaryOperation(ast* tree, symTable* st);


/**
 * @brief genSymTable_unaryOperation generate the symbol table from the given AST for an unary operation
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_unaryOperation(ast* tree, symTable* st);


/**
 * @brief genSymTable_declaration generate the symbol table from the given AST for a variable declaration
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_declaration(ast* tree, symTable* st);


/**
 * @brief genSymTable_functionDeclaration generate the symbol table from the given AST for an function declaration
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_functionDeclaration(ast* tree, symTable* st);


/**
 * @brief genSymTable_aux_newArg generate argument object from the given symbol
 * @param smbl
 **/
args* genSymTable_aux_newArg(symbol* smbl);


/**
 * @brief genSymTable_functionCall generate the symbol table from the given AST for an function call
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_functionCall(ast* tree, symTable* st);


/**
 * @brief genSymTable_tableDeclaration generate the symbol table from the given AST for an table declaration
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_tableDeclaration(ast* tree, symTable* st);


/**
 * @brief genSymTable_aux_newDim generate dimension object with the given value
 * @param value
 **/
dims* genSymTable_aux_newDim(int value);


/**
 * @brief checkElementsNumberInTableDeclaration check if the number elements of the
 *  table declaration does not exceed the table dimensions
 * @param dimensions table dimensions
 * @param elementsBlock blocks of elements of the table
 **/
bool checkElementsNumberInTableDeclaration(dims* dimensions, ast* elementsBlock, symTable* st);


/**
 * @brief genSymTable_tableAccess generate the symbol table from the given AST for an table access
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_tableAccess(ast* tree, symTable* st);



/**
 * @brief genSymTable_newTemp create the newTemp int the symbol table and change the temporary variable by the symbol identifier
 * @param tree AST
 * @param st symbol table
 **/
symbol* genSymTable_newTemp(ast* tree, symTable* st);


/**
 * @brief genSymTable_const create the const variable in the symbol table
 * @param tree AST
 * @param st symbol table
 **/
symbol* genSymTable_const(ast* tree, symTable* st);


#endif
