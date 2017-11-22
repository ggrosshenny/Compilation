
#include "quad.h"

quad* quad_init(){
  quad *newQuad = malloc(sizeof(quad));
  newQuad->arg1 = NULL;
  newQuad->arg2 = NULL;
  newQuad->res = NULL;
  newQuad->next = NULL;
  return newQuad;
}

quadList* quadList_init(){
  quadList* newQuadList = malloc(sizeof(quadList));
  newQuadList->list = NULL;
  newQuadList->nextQuad = 0;
  newQuadList->last = NULL;

  return newQuadList;
}

void init_existing_quad(quad* newQuad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result){
  newQuad = malloc(sizeof(quad));

  newQuad->type = type_ast;
  newQuad->arg1 = arg_1;
  newQuad->arg2 = arg_2;
  newQuad->res = result;
  newQuad->next = NULL;
}

codegen* codegen_init(){
  codegen *newCg = malloc(sizeof(codegen));
  newCg->result = NULL;
  newCg->code = quadList_init();
  return newCg;
}


void quad_free(quad* quadToBeFreed){
  free(quadToBeFreed->arg1);
  free(quadToBeFreed->arg2);
  free(quadToBeFreed->res);
  free(quadToBeFreed->next);
  free(quadToBeFreed);
}


void quadList_free_keepList(quadList* ql)
{
  free(ql);
}


void quad_add(quadList* last_quad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result){
  quad *newQuad = NULL;

  init_existing_quad(newQuad, type_ast, arg_1, arg_2, result);

  if(last_quad->list == NULL)
  {
    last_quad->list = newQuad;
    last_quad->last = last_quad->list;
  }
  else
  {
      last_quad->last->next = newQuad;
      last_quad->last = last_quad->last->next;
  }

}

void concat(quadList* q1, quadList* q2){
  if(q1->list != NULL && q2->list != NULL)
  {
    q1->last->next = q2->list;
    q1->last = q2->last;
    q1->nextQuad += q2->nextQuad;
    quadList_free_keepList(q2);
  }
}

void codegen_ast_operations(codegen* cg, enum ast_type type, codegen* left, codegen* right, symTable* symbol_table){
  symbol* s = symTable_newTemp(symbol_table, 0);
  cg->result = s;
  
  if(right != NULL)
    concat(left->code, right->code);
  concat(cg->code, left->code);

  if(right != NULL)
    quad_add(cg->code, type, left->result, right->result, cg->result);
  else
    quad_add(cg->code, type, left->result, NULL, cg->result);

}

codegen* codegen_ast(ast* ast, symTable* symbol_table){

  codegen* cg = codegen_init();
  codegen* left = codegen_init();
  codegen* right = codegen_init();
  //symbol* s;
  switch(ast->type){
    case AST_ID:
      printf("ID \n");
      cg->result = symTable_lookUp(symbol_table, ast->component.identifier);
      break;

    case AST_OP_ADD:
      printf("add \n");
      left = codegen_ast(ast->component.operation.left, symbol_table);
      right = codegen_ast(ast->component.operation.right, symbol_table);

      codegen_ast_operations(cg, AST_OP_ADD, left, right, symbol_table);

      break;

    case AST_OP_SUB:

          left = codegen_ast(ast->component.operation.left, symbol_table);
          right = codegen_ast(ast->component.operation.right, symbol_table);

          codegen_ast_operations(cg, AST_OP_SUB, left, right, symbol_table);

      break;

    case AST_OP_MULT:
          printf("mult \n");
          left = codegen_ast(ast->component.operation.left, symbol_table);
          right = codegen_ast(ast->component.operation.right, symbol_table);

          codegen_ast_operations(cg, AST_OP_MULT, left, right, symbol_table);

      break;

    case AST_OP_DIV:

          left = codegen_ast(ast->component.operation.left, symbol_table);
          right = codegen_ast(ast->component.operation.right, symbol_table);

          codegen_ast_operations(cg, AST_OP_DIV, left, right, symbol_table);

      break;

    case AST_OP_MINUS:
      printf("minus\n");
      left = codegen_ast(ast->component.operation.left, symbol_table);
      right = NULL;

      codegen_ast_operations(cg, AST_OP_MINUS, left, NULL, symbol_table);
      break;


    default:
      break;
  }
  return cg;
}
