
#include "quad.h"

quad* quad_init(){
  quad *newQuad = calloc(1, sizeof(quad));
  newQuad->arg1 = NULL;
  newQuad->arg2 = NULL;
  newQuad->res = NULL;
  newQuad->next = NULL;
  return newQuad;
}

quadList* quadList_init(){
  quadList* newQuadList = calloc(1, sizeof(quadList));
  newQuadList->list = NULL;
  newQuadList->nextQuad = 0;
  newQuadList->last = NULL;

  return newQuadList;
}

quad* init_existing_quad(enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result){
  quad* newQuad = calloc(1, sizeof(quad));

  newQuad->type = type_ast;
  newQuad->arg1 = arg_1;
  newQuad->arg2 = arg_2;
  newQuad->res = result;
  newQuad->next = NULL;

  return newQuad;
}

void quadList_print(quadList* ql){
  quad* temp = ql->list;
  printf("QUADLIST'S TYPES: ");
  while(temp != NULL){
    switch(temp->type)
    {
      case AST_INT     :  printf("AST_INT "); break;
      case AST_ID      :  printf("AST_ID "); break;
      case AST_OP_ADD  :  printf("AST_OP_ADD "); break;
      case AST_OP_SUB  :  printf("AST_OP_SUB "); break;
      case AST_OP_MULT :  printf("AST_OP_MULT "); break;
      case AST_OP_DIV  :  printf("AST_OP_DIV "); break;
      case AST_OP_AFCT  : printf("AST_OP_AFCT "); break;
      case AST_OP_INCR  : printf("AST_OP_INCR "); break;
      case AST_OP_DECR  : printf("AST_OP_DECR "); break;
      case AST_OP_MINUS : printf("AST_OP_MINUS "); break;
      case AST_FUNC_DEF : printf("AST_FUNC_DEF "); break;
      case AST_FUNC_CALL: printf("AST_FUNC_CALL "); break;
      default         :   break;
    }
    temp = temp->next;
  }
  printf("\n");
}

codegen* codegen_init(){
  codegen *newCg = calloc(1, sizeof(codegen));
  newCg->result = NULL;
  newCg->code = quadList_init();
  return newCg;
}


void quad_free(quad* quadWantsToBeFreed){
  free(quadWantsToBeFreed);
}


void quadList_free_keepList(quadList* ql){
  ql->list = NULL;
  ql->last = NULL;
  free(ql);
}

void quadList_free(quadList* ql){

  quad* temp;
  while(ql->list != NULL){
    temp = ql->list;
    ql->list = ql->list->next;
    quad_free(temp);
  }
  free(ql);
}

void codegen_free(codegen* cg){

  if(cg->code != NULL){
    quadList_free(cg->code);
  }
  free(cg);
}

void codegen_keepQuadList_free(codegen *cg){
  free(cg);
}


void quad_add(quadList* last_quad, enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result){
  quad *newQuad = init_existing_quad(type_ast, arg_1, arg_2, result);

  if(last_quad->list == NULL){
    last_quad->list = newQuad;
    last_quad->last = last_quad->list;
  }
  else{
      last_quad->last->next = newQuad;
      last_quad->last = last_quad->last->next;
  }

}


quadList* concat(quadList* q1, quadList* q2){
  quadList* newQl = q1;

  if((newQl->list != NULL) && (q2->list != NULL)){

    newQl->last->next = q2->list;
    newQl->last = q2->last;
    newQl->nextQuad += q2->nextQuad;
    free(q2);
  }
  else{
    if(newQl->list == NULL){

      newQl = q2;
      free(q1);
    }
  }
  return newQl;
}


void codegen_ast_operations(codegen* cg, enum ast_type type, codegen* left, codegen* right, symTable* symbol_table){
  symbol* s = symTable_newTemp(symbol_table, 0);
  cg->result = s;

  if(right != NULL)
  {
    left->code = concat(left->code, right->code);
  }

  cg->code = concat(cg->code, left->code);

  if(right != NULL)
  {
    quad_add(cg->code, type, left->result, right->result, cg->result);
  }
  else
  {
    quad_add(cg->code, type, left->result, NULL, cg->result);
  }

}

codegen* codegen_ast(codegen* cg, ast* ast, symTable* symbol_table){

  codegen* left = codegen_init();
  codegen* right = codegen_init();

  switch(ast->type){

    case AST_INT:
      cg->result = symTable_newTemp(symbol_table, ast->component.number);
      quadList_free_keepList(left->code);
      quadList_free_keepList(right->code);
      break;

    case AST_ID:
      cg->result = symTable_lookUp(symbol_table, ast->component.identifier);
      quadList_free_keepList(left->code);
      quadList_free_keepList(right->code);
      break;

    case AST_OP_ADD:
      left = codegen_ast(left, ast->component.operation.left, symbol_table);
      right = codegen_ast(right, ast->component.operation.right, symbol_table);

      codegen_ast_operations(cg, AST_OP_ADD, left, right, symbol_table);

      break;

    case AST_OP_SUB:

      left = codegen_ast(left, ast->component.operation.left, symbol_table);
      right = codegen_ast(right, ast->component.operation.right, symbol_table);

      codegen_ast_operations(cg, AST_OP_SUB, left, right, symbol_table);

      break;

    case AST_OP_MULT:
      left = codegen_ast(left, ast->component.operation.left, symbol_table);
      right = codegen_ast(right, ast->component.operation.right, symbol_table);

      codegen_ast_operations(cg, AST_OP_MULT, left, right, symbol_table);

      break;

    case AST_OP_DIV:

      left = codegen_ast(left, ast->component.operation.left, symbol_table);
      right = codegen_ast(right, ast->component.operation.right, symbol_table);

      codegen_ast_operations(cg, AST_OP_DIV, left, right, symbol_table);

      break;

    case AST_OP_MINUS:
      left = codegen_ast(left, ast->component.operation.left, symbol_table);
      quadList_free_keepList(right->code);

      codegen_ast_operations(cg, AST_OP_MINUS, left, NULL, symbol_table);
      break;


    default:
      break;
  }

  codegen_keepQuadList_free(left);
  codegen_keepQuadList_free(right);
  return cg;
}
