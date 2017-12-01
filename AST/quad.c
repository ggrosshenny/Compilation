#include "quad.h"

quad* quad_init(){
  quad *newQuad = calloc(1, sizeof(quad));
  newQuad->arg1 = NULL;
  newQuad->arg2 = NULL;
  newQuad->res = NULL;
  newQuad->next = NULL;
  newQuad->index = 0;
  return newQuad;
}

quadList* quadList_init(){
  quadList* newQuadList = calloc(1, sizeof(quadList));
  newQuadList->list = NULL;
  newQuadList->last = NULL;
  newQuadList->indexNextQuad = 0;

  return newQuadList;
}

quad* init_existing_quad(enum ast_type type_ast, symbol* arg_1, symbol* arg_2, symbol* result, int quadIndex){
  quad* newQuad = calloc(1, sizeof(quad));

  newQuad->type = type_ast;
  newQuad->arg1 = arg_1;
  newQuad->arg2 = arg_2;
  newQuad->res = result;
  newQuad->index = quadIndex;
  newQuad->next = NULL;

  return newQuad;
}

void quadList_print(quadList* ql){
  quad* temp = ql->list;
  printf("QUADLIST'S TYPES:\n");
  while(temp != NULL){
    switch(temp->type)
    {
      case AST_INT     :  printf("\tAST_INT, index : %d\n", temp->index); break;
      case AST_ID      :  printf("\tAST_ID, index : %d\n", temp->index); break;
      case AST_OP_ADD  :  printf("\tAST_OP_ADD, index : %d\n", temp->index); break;
      case AST_OP_SUB  :  printf("\tAST_OP_SUB, index : %d\n", temp->index); break;
      case AST_OP_MULT :  printf("\tAST_OP_MULT, index : %d\n", temp->index); break;
      case AST_OP_DIV  :  printf("\tAST_OP_DIV, index : %d\n", temp->index); break;
      case AST_OP_AFCT  : printf("\tAST_OP_AFCT, index : %d\n", temp->index); break;
      case AST_OP_DECL  : printf("\tAST_OP_DECL, index : %d\n", temp->index); break;
      case AST_OP_INCR  : printf("\tAST_OP_INCR, index : %d\n", temp->index); break;
      case AST_OP_DECR  : printf("\tAST_OP_DECR, index : %d\n", temp->index); break;
      case AST_OP_MINUS : printf("\tAST_OP_MINUS, index : %d\n", temp->index); break;
      case AST_FUNC_DEF : printf("\tAST_FUNC_DEF, index : %d\n", temp->index); break;
      case AST_FUNC_CALL: printf("\tAST_FUNC_CALL, index : %d\n", temp->index); break;
      case AST_FUNC_BODY: printf("\tAST_FUNC_BODY, index : %d\n", temp->index); break;
      case AST_FUNC_ARG : printf("\tAST_FUNC_ARG, index : %d, val : %d\n", temp->index, temp->res->content.val.integer); break;
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
  last_quad->indexNextQuad++;
  quad *newQuad = init_existing_quad(type_ast, arg_1, arg_2, result, last_quad->indexNextQuad);

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
  quad* temp = q2->list;

  if((newQl->list != NULL) && (q2->list != NULL)){

    newQl->last->next = q2->list;
    newQl->last = q2->last;
    while(temp != NULL)
    {
      newQl->indexNextQuad++;
      temp->index = newQl->indexNextQuad;
      temp = temp->next;
    }
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
  value newVal;
  newVal.integer = 0;
  symbol* s = symTable_newTemp(symbol_table, INT, newVal);
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


void codegen_ast_affectation(codegen* cg, enum ast_type type, codegen* left, codegen* right)
{
  cg->result = left->result;

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


void codegen_ast_functionBody(codegen* cg, codegen* instruction, codegen* nextInstruction, ast* ast, symTable* symbol_table)
{
  instruction = codegen_ast(instruction, ast->component.instructionsList.instruction, symbol_table);
  if(ast->component.instructionsList.nextInstruction != NULL)
  {
    nextInstruction = codegen_ast(nextInstruction, ast->component.instructionsList.nextInstruction, symbol_table);
    instruction->code = concat(instruction->code, nextInstruction->code);
  }
  else
  {
    quadList_free_keepList(nextInstruction->code);
  }
  // Just concat the codes of the instructions
  cg->code = concat(cg->code, instruction->code);

}


void codegen_ast_functionArguments(codegen* cg, codegen* argument, codegen* nextArgument, ast* ast, symTable* symbol_table)
{
  argument = codegen_ast(argument, ast->component.argumentsList.argument, symbol_table);
  if(ast->component.argumentsList.nextArg != NULL)
  {
    nextArgument = codegen_ast(nextArgument, ast->component.argumentsList.nextArg, symbol_table);
  }
  else
  {
    quadList_free_keepList(nextArgument->code);
  }

  cg->result = argument->result;

  if(ast->component.argumentsList.nextArg != NULL)
  {
    quad* temp;
    quadList* qlTemp;
    quad_add(argument->code, AST_FUNC_ARG, argument->result, nextArgument->result, cg->result);

    temp = argument->code->list;
    while((temp->next != argument->code->last) && (temp->next != NULL))
    {
      temp = temp->next;
    }
    argument->code->last->next = argument->code->last;
    argument->code->last = nextArgument->code->list;
    temp->next = argument->code->last;

    qlTemp = cg->code;
    cg->code = argument->code;
    quadList_free_keepList(qlTemp);
    quadList_free_keepList(nextArgument->code);
  }
  else
  {
    quad_add(cg->code, AST_FUNC_ARG, argument->result, NULL, cg->result);
    quadList_free_keepList(argument->code);
  }

}


void codegen_ast_functionCall(codegen* cg, codegen* arguments, codegen* identifier, ast* ast, symTable* symbol_table)
{
  // WARNING : ONLY good for printi and printf !
  //           need to find a way to take multiple arguments, stor it and use it
  //           Exemple : les mettre dans la table des symboles lors de la def de la fonctions
  //           ou bien créer un temp dès qu'on rencontre un argument
  if(ast->component.function.arguments != NULL)
  {
    arguments = codegen_ast(arguments, ast->component.function.arguments, symbol_table);
    arguments->code = concat(arguments->code, identifier->code);
  }
  else
  {
    quadList_free_keepList(arguments->code);
  }

  identifier = codegen_ast(identifier, ast->component.function.identifier, symbol_table);

  quadList_free_keepList(identifier->code);
  cg->code = concat(cg->code, arguments->code);
  cg->result = identifier->result;

  printf("id : %s\n", cg->result->identifier);

  if(ast->component.function.arguments != NULL)
  {
    quad_add(cg->code, AST_FUNC_CALL, NULL, NULL, cg->result);
  }
  else
  {
    quad_add(cg->code, AST_FUNC_CALL, arguments->result, NULL, cg->result);
  }

}


codegen* codegen_ast(codegen* cg, ast* ast, symTable* symbol_table){

  codegen* left = codegen_init();
  codegen* right = codegen_init();
  value newVal;

  if(ast != NULL)
  {
    switch(ast->type){

      case AST_INT:
        newVal.integer = ast->component.number;
        cg->result = symTable_newTemp(symbol_table, INT, newVal);
        quadList_free_keepList(left->code);
        quadList_free_keepList(right->code);
        break;

      case AST_STR:
        newVal.string = ast->component.string;
        cg->result = symTable_newTemp(symbol_table, STRING, newVal);
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


      case AST_OP_AFCT:

        left = codegen_ast(left, ast->component.operation.left, symbol_table);
        right = codegen_ast(right, ast->component.operation.right, symbol_table);

        codegen_ast_affectation(cg, AST_OP_AFCT, left, right);
        break;

      case AST_OP_DECL:

        left = codegen_ast(left, ast->component.operation.left, symbol_table);
        quadList_free_keepList(right->code);
        quadList_free_keepList(left->code);
        cg->result = left->result;

        //codegen_ast_operations(cg, AST_OP_DECL, left, NULL, symbol_table);
        break;

      // Functions
      case AST_FUNC_DEF:
        // Need to create function "genCode_createFunc" for ID and Arguments section in function AST
        left = codegen_ast(left, ast->component.function.identifier, symbol_table);
        right = codegen_ast(right, ast->component.function.body, symbol_table);

        codegen_ast_operations(cg, AST_FUNC_DEF, left, NULL, symbol_table);
        cg->code = concat(cg->code, right->code);
        break;

      case AST_FUNC_BODY:
        codegen_ast_functionBody(cg, left, right, ast, symbol_table);
        break;

      case AST_FUNC_ARG:
        // Need to take care of number of arguments in quadToMIPS with the list of args in the symbol table !
        codegen_ast_functionArguments(cg, left, right, ast, symbol_table);
        break;

      case AST_FUNC_CALL:
        codegen_ast_functionCall(cg, left, right, ast, symbol_table);
        break;

      default:
        quadList_free(left->code);
        quadList_free(right->code);
        quadList_free(cg->code);
        break;
    }
  }

  if(ast==NULL)
  {
    quadList_free_keepList(left->code);
    quadList_free_keepList(right->code);
  }

  codegen_keepQuadList_free(left);
  codegen_keepQuadList_free(right);
  return cg;
}
