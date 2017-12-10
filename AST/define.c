#include <stdio.h>
#include <stdlib.h>
#include "define.h"


defineList* new_define(char* identifier0, ast* value0){
  defineList* newDefList = calloc(1, sizeof(defineList));
  newDefList->identifier = strndup(identifier0, MAX_IDENTIFIER_LENGHT);
  newDefList->value = value0;
  newDefList->next = NULL;
  return newDefList;
}


void free_defineList(defineList* list){
  defineList* temp;
  while(list != NULL){
    temp = list;
    list =list->next;
    free(temp->identifier);
    ast_free(temp->value);
    free(temp);
  }
}


defineList* search_existingDefine(defineList* list, const char* identifier0){

  defineList* temp = list;
  while(temp != NULL){
      if(strcmp(identifier0, temp->identifier) == 0){
        return temp;
      }
      else{
        temp = temp->next;
      }
  }
  return NULL;
}


void add_define_to_list(defineList* list, defineList* def){
  defineList* temp = list;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = def;
}
