#ifndef __ASTTOST_H_
#define __ASTTOST_H_

#include "ast.h"
#include "symbolTable.h"


/**
 * @brief genSymTable_ast generate the symbol table from the given AST
 * @param tree AST
 * @param st symbol table
 **/
ast* genSymTable_ast(ast* tree, symTable* st);


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
