#ifndef __DEFINE_H_
#define __DEFINE_H_

#include "ast.h"


// Structure definition : chained list of preprocessed const
typedef struct s_define{
  char* identifier;     // Indentifier of the const
  ast* value;           // Value of the const (instructions list/number)
  struct s_define* next;// Next block of the chained list
}defineList;

/**
 * @brief new_define Creates a new preprocessed const
 * @param identifier0 Identifier of the const
 * @param value0  Value of the const (AST)
 **/
defineList* new_define(char* identifier0, ast* value0);


/**
 * @brief free_defineList Free the memory allocated to a chained list of preprocessed consts
 * @param list List to be freed
 **/
void free_defineList(defineList* list);


/**
 * @brief search_existingDefine Travel through a preprocessed const list to find one by its identifier (return NULL if id doesn't exist in the list)
 * @param list List where the const has to be found
 * @param identifier0 Identifier of the const to be found
 **/
defineList* search_existingDefine(defineList* list, const char* identifier0);


/**
 * @brief add_define_to_list Add a preprocessed const at the end of a chained list
 * @param list List where the const has to be added
 * @param def preprocessed to be added to the chained list
 **/
void add_define_to_list(defineList* list, defineList* def);




#endif
