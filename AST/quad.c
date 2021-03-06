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
      case AST_GOTO     : printf("\tAST_GOTO -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_CREATE_LABEL : printf("\tAST_CREATE_LABEL : %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_EQ  : printf("\tAST_BOOL_EQ -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_NEQ : printf("\tAST_BOOL_NEQ -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_GT  : printf("\tAST_BOOL_GT -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_LT  : printf("\tAST_BOOL_LT -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_GEQ : printf("\tAST_BOOL_GEQ -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_BOOL_LEQ : printf("\tAST_BOOL_LEQ -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_FUNC_DEF : printf("\tAST_FUNC_DEF -> %s, index : %d\n", temp->res->identifier, temp->index); break;
      case AST_FUNC_CALL: printf("\tAST_FUNC_CALL, index : %d\n", temp->index); break;
      case AST_FUNC_BODY: printf("\tAST_FUNC_BODY, index : %d\n", temp->index); break;
      case AST_FUNC_ARG : printf("\tAST_FUNC_ARG, index : %d, val : %d\n", temp->index, temp->res->content.val.integer); break;
      case AST_TAB_AFCT : printf("\tAST_TAB_AFCT : %d, index : %d\n", temp->arg1->content.val.integer, temp->index); break;
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

  if(ast->component.function.arguments != NULL)
  {
    quad_add(cg->code, AST_FUNC_CALL, NULL, NULL, cg->result);
  }
  else
  {
    quad_add(cg->code, AST_FUNC_CALL, arguments->result, NULL, cg->result);
  }

}


void codegen_ast_boolExpression(codegen* cg, ast* ast, symTable* symbol_table, char* trueBlock_label, char* falseBlock_label)
{
  if(ast != NULL)
  {
    codegen* true_code = codegen_init();
    codegen* false_code = codegen_init();
    codegen* bool_arg1 = codegen_init();
    codegen* bool_arg2 = codegen_init();
    symbol* true_goto = NULL;
    symbol* false_goto = NULL;
    symbol* skip_goto = NULL;

    // Generate quad for the true part
    if(ast->component.boolean.ast_true->type == AST_GOTO)
    {
      true_goto = symTable_lookUp(symbol_table, trueBlock_label);
    }
    else
    {
      codegen_ast_boolExpression(true_code, ast->component.boolean.ast_true, symbol_table, trueBlock_label, falseBlock_label);
      true_goto = symTable_addLabel(symbol_table, "true_label", TRUE);
    }

    // Generate quad for the false part
    if(ast->component.boolean.ast_false->type == AST_GOTO)
    {
      false_goto = symTable_lookUp(symbol_table, falseBlock_label);
    }
    else
    {
      codegen_ast_boolExpression(false_code, ast->component.boolean.ast_false, symbol_table, trueBlock_label, falseBlock_label);
      false_goto = symTable_addLabel(symbol_table, "false_label", FALSE);
    }

    // Generate quad for the boolean expression and add ot to the beginning of the quads list
    codegen_ast(bool_arg1, ast->component.boolean.boolExpr->component.operation.left, symbol_table); // code = NULL
    codegen_ast(bool_arg2, ast->component.boolean.boolExpr->component.operation.right, symbol_table); // code = NULL
    quad_add(cg->code, ast->component.boolean.boolExpr->type, bool_arg1->result, bool_arg2->result, true_goto);
      // Add the jump to the false_label
    quad_add(cg->code, AST_GOTO, NULL, NULL, false_goto);
      // Add the TRUE instructions block
    if(ast->component.boolean.ast_true->type != AST_GOTO)
    {
      quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, true_goto);
      cg->code = concat(cg->code, true_code->code);
    }
    else
    {
      quadList_free_keepList(true_code->code);
    }
      // Create the skip label and add the GOTO quad
    if((ast->component.boolean.ast_true->type != AST_GOTO) || (ast->component.boolean.ast_false->type != AST_GOTO))
    {
      skip_goto = symTable_addLabel(symbol_table, "skip_label", SKIP);
      quad_add(cg->code, AST_GOTO, NULL, NULL, skip_goto);
    }
      // create the quad to create the false label
    if(ast->component.boolean.ast_false->type != AST_GOTO)
    {
      quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, false_goto);
        // Add the FALSE instructions block
      cg->code = concat(cg->code, false_code->code);
    }
    else
    {
      quadList_free_keepList(false_code->code);
    }
      // Create the quad to create the skip label
    if((ast->component.boolean.ast_true->type != AST_GOTO) || (ast->component.boolean.ast_false->type != AST_GOTO))
    {
      quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, skip_goto);
    }

    // Free codegens and quadLists objects
    codegen_keepQuadList_free(true_code);
    codegen_keepQuadList_free(false_code);
    quadList_free_keepList(bool_arg1->code);
    codegen_keepQuadList_free(bool_arg1);
    quadList_free_keepList(bool_arg2->code);
    codegen_keepQuadList_free(bool_arg2);
  }
}


void codegen_ast_controlStructure(codegen* cg, codegen* conditions, codegen* true_instruction, ast* ast, symTable* symbol_table)
{
  codegen* false_instruction = codegen_init();
  codegen* varInitialization;
  codegen* varUpdate;
  symbol* main_true_label = 0;
  symbol* main_false_label = 0;
  symbol* main_skip_label = 0;
  symbol* main_loop_label = 0;
  // Create GOTO labels for true, false and skip jump
    // True_label_xx
  main_true_label = symTable_addLabel(symbol_table, "true_label", TRUE);
    // False_label_xx
  main_false_label = symTable_addLabel(symbol_table, "false_label", FALSE);

  // If the loop is a FOR loop
  if(ast->type == AST_FOR)
  {
    varInitialization = codegen_init();
    varUpdate = codegen_init();
    codegen_ast(varInitialization, ast->component.boolean.forVarInit, symbol_table);
    codegen_ast(varUpdate, ast->component.boolean.forVarUpdate, symbol_table);
  }

  // Generat quads for the boolean expression
  codegen_ast_boolExpression(conditions, ast->component.boolean.boolExpr, symbol_table, main_true_label->identifier, main_false_label->identifier);

  // Generate code for TRUE instructions block
  true_instruction = codegen_ast(true_instruction, ast->component.boolean.ast_true, symbol_table);

  // Generate code for FALSE instructions block
  if(ast->component.boolean.ast_false !=NULL)
  {
      // Skip_label_xx
    main_skip_label = symTable_addLabel(symbol_table, "skip_label", SKIP);
    false_instruction = codegen_ast(false_instruction, ast->component.boolean.ast_false, symbol_table);
  }


  // If the structure is a loop : add a label to jump back
  if((ast->type == AST_WHILE) || (ast->type == AST_FOR))
  {
    if(ast->type == AST_FOR)
    {
      cg->code = concat(cg->code, varInitialization->code);
    }
      // loop_label_xx
    main_loop_label = symTable_addLabel(symbol_table, "loop_label", LOOP);
    quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, main_loop_label);
  }

  // Concat the codes in the good order
    // Add the boolean expression
  cg->code = concat(cg->code, conditions->code);

    // Add the quad to create the main_true_label
  quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, main_true_label);

    // Add the TRUE instructions block
  cg->code = concat(cg->code, true_instruction->code);

  if(ast->component.boolean.ast_false !=NULL)
  {
      // Add the goto quad to main_skip_label
    quad_add(cg->code, AST_GOTO, NULL, NULL, main_skip_label);
  }

  if((ast->type == AST_WHILE) || (ast->type == AST_FOR))
  {
    if(ast->type == AST_FOR)
    {
      cg->code = concat(cg->code, varUpdate->code);
    }
    quad_add(cg->code, AST_GOTO, NULL, NULL, main_loop_label);
  }

    // Add the quad to create the main_false_label
  quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, main_false_label);


  if(ast->component.boolean.ast_false !=NULL)
  {
      // Add the FALSE instructions block
    cg->code = concat(cg->code, false_instruction->code);
      // Add the quad to create the main_skip_label
    quad_add(cg->code, AST_CREATE_LABEL, NULL, NULL, main_skip_label);
  }
  else
  {
    quadList_free_keepList(false_instruction->code);
  }

  codegen_keepQuadList_free(false_instruction);
  if(ast->type == AST_FOR)
  {
    codegen_keepQuadList_free(varInitialization);
    codegen_keepQuadList_free(varUpdate);
  }
}


void codegen_ast_tableDeclaration(codegen* cg, ast* ast, symTable* symbol_table)
{
  symbol* tabSymbol = symTable_lookUp(symbol_table, ast->component.tableDeclaration.identifier->component.identifier);

  if(ast->component.tableDeclaration.elements != NULL)
  {
    codegen_ast_tableDeclarationVal(cg, ast->component.tableDeclaration.elements, tabSymbol->content.val.dimensions, tabSymbol, symbol_table, 0);
  }
}





void codegen_ast_tableDeclarationVal(codegen* cg, ast* elements, dims* dimensions, symbol* tabSymbol, symTable* symbol_table, int lastBlockAdr)
{
  dims* tempDim = dimensions->nextDim;
  int dimVal = dimensions->currentDim;
  int dimSizesMultiplication = 1;
  ast* currentElem = elements;
  codegen* valueCG = codegen_init();
  symbol* indice = NULL;
  value val;

  // Compute dimSizesMultiplication
  while(tempDim != NULL)
  {
    dimSizesMultiplication *= tempDim->currentDim;
    tempDim = tempDim->nextDim;
  }

  // find adress for all elements in block
  switch(currentElem->type)
  {
    // If the dimension is not the final one
    case AST_TAB_BLCK :
      for(int i=0; i<dimVal; i++)
      {
        codegen_ast_tableDeclarationVal(cg, currentElem->component.tableElementsBlock.elements, dimensions->nextDim, tabSymbol, symbol_table, lastBlockAdr + (i * dimSizesMultiplication));
        currentElem = currentElem->component.tableElementsBlock.nextBlock;
      }
      break;
    case AST_TAB_ELEM :
      for(int i=0; i<dimVal; i++)
      {
        val.integer = (lastBlockAdr + i) * WORDSIZE;
        codegen_ast(valueCG, currentElem->component.tableElementsList.currentElem, symbol_table);
        indice = symTable_addTabElemAdr(symbol_table, INT, val);
        quad_add(cg->code, AST_TAB_AFCT, indice, valueCG->result, tabSymbol);
        currentElem = currentElem->component.tableElementsList.nextElem;
      }
    default :
      break;
  }

  codegen_free(valueCG);
}


void codegen_ast_tableAccess(codegen* cg, ast* tree, symTable* symbol_table)
{
  symbol* tabSymbol = symTable_lookUp(symbol_table, tree->component.tableAccess.identifier->component.identifier);
  symbol* elementAdrSymbol = NULL;
  dims* dimensions = tabSymbol->content.val.dimensions;
  ast* indices = tree->component.tableAccess.indices;
  dims* tempDim = NULL;
  int dimSizesMultiplication = 1;
  int elementAdr = 0;
  char* tabAcessForMIPSUsage = calloc(256, sizeof(char));
  value val;

  // Generate the quad
  while(indices->component.tableDimensionsList.nextDim != NULL)
  {
    tempDim = dimensions->nextDim;
    dimSizesMultiplication = 1;
    while(tempDim != NULL)
    {
      dimSizesMultiplication *= tempDim->currentDim;
      tempDim = tempDim->nextDim;
    }

    elementAdr += indices->component.tableDimensionsList.val->component.number * dimSizesMultiplication;

    dimensions = dimensions ->nextDim;
    indices = indices->component.tableDimensionsList.nextDim;
  }

  tempDim = dimensions->nextDim;
  while(tempDim != NULL)
  {
    dimSizesMultiplication *= tempDim->currentDim;
    tempDim = tempDim->nextDim;
  }

  elementAdr += indices->component.tableDimensionsList.val->component.number;
  elementAdr *= WORDSIZE;

  snprintf(tabAcessForMIPSUsage, 256, "%d %s", elementAdr, tabSymbol->identifier);
  val.string = tabAcessForMIPSUsage;
  elementAdrSymbol = symTable_addTabElemAdr(symbol_table, STRING, val);
  cg->result = elementAdrSymbol;

}


void codegen_ast_stencilOperation(codegen* cg, ast* tree, symTable* symbol_table)
{
  ast* stencil = tree->component.operation.left;
  ast* table = tree->component.operation.right;
  symbol* tabSym = symTable_lookUp(symbol_table, table->component.tableAccess.identifier->component.identifier);
  symbol* stenSym = symTable_lookUp(symbol_table, stencil->component.identifier);
  dims* tabDim = tabSym->content.val.dimensions;
  dims* stenDim = stenSym->content.val.dimensions;
  ast* tabIndices = table->component.tableAccess.indices;

  codegen_ast_stencilOperationAux(cg, tabIndices, tabDim, stenDim, tabSym, stenSym, 0, 0, symbol_table);
}


// Warning, very long function ! :(
void codegen_ast_stencilOperationAux(codegen* cg, ast* tabIndices, dims* tabDim, dims* stenDim, symbol* tabSym, symbol* stenSym, int tabLastBlockAdr, int stenLastBlockAdr, symTable* symbol_table)
{
  symbol* temporaryVar = NULL;
  symbol* temporaryVarBis = NULL;
  symbol* stenElemAdr = NULL;
  symbol* tabElemAdr = NULL;
  value val0;
  codegen* temp = NULL;
  int stenDimSizesMultiplication = 1;
  int tabDimSizesMultiplication = 1;
  int stenDimSize = 0;
  int tabDimSize = 0;
  int tabShiftVal = 0;
  int currentDimensionElemIndice = 0;
  dims* tempStenDims = NULL;
  dims* tempTabDims = NULL;
  value val;
  char* tabAcessForMIPSUsage = NULL;

  // First case : table and stencil has the same dimensions
  if((stenDim->nextDim != NULL) && (tabDim->nextDim != NULL))
  {
    tempStenDims = stenDim->nextDim;
    tempTabDims = tabDim->nextDim;

    // Compute stenDimSizesMultiplication
    while(tempStenDims != NULL)
    {
      stenDimSizesMultiplication *= tempStenDims->currentDim;
      tempStenDims = tempStenDims->nextDim;
    }

    // Compute tabDimSizesMultiplication
    while(tempTabDims != NULL)
    {
      tabDimSizesMultiplication *= tempTabDims->currentDim;
      tempTabDims = tempTabDims->nextDim;
    }

    // Find table and stencil current dimension size
    stenDimSize = stenDim->currentDim;
    tabDimSize = tabDim->currentDim;

    // Compute value shift on the table
    tabShiftVal = stenDimSize/2;

    // find adresses for the stencil applications on the table
    // to find the (i,j) pixel value
    currentDimensionElemIndice = tabIndices->component.tableDimensionsList.val->component.number;
    for(int i=0; i<stenDimSize; i++)
    {
      temp = codegen_init();
      // First case, the aimed value indice in the current dimension is less than 0
      if((currentDimensionElemIndice - tabShiftVal + i) < 0)
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr, stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }
      // Second case, the aimed value indice in the current dimension is greater than the table current dimension size
      else if((currentDimensionElemIndice - tabShiftVal + i) > (tabDim->currentDim -1))
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr  + ((tabDim->currentDim - 1) * tabDimSizesMultiplication), stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }
      // Third case, the aimed value indice in the current dimension is in the current dimension
      else
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr + ((currentDimensionElemIndice - tabShiftVal + i) * tabDimSizesMultiplication), stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }

      // Concat producted quads

      temporaryVar = cg->result;
      cg->code = concat(cg->code, temp->code);

      if(i>0)
      {
        val0.integer = 0;
        temporaryVarBis = symTable_newTemp(symbol_table, INT, val0);
        quad_add(cg->code, AST_OP_ADD, temporaryVar, temp->result, temporaryVarBis);
        cg->result = temporaryVarBis;
      }
      else
      {
        cg->result = temp->result;
      }

      codegen_keepQuadList_free(temp);
    }
  }
  // Second case : table has  a biger amount of dimensions than stencil
  else if((stenDim->nextDim == NULL) && (tabDim->nextDim != NULL))
  {
    tempTabDims = tabDim->nextDim;

    // Compute tabDimSizesMultiplication
    while(tempTabDims != NULL)
    {
      tabDimSizesMultiplication *= tempTabDims->currentDim;
      tempTabDims = tempTabDims->nextDim;
    }

    // Find table and stencil current dimension size
    stenDimSize = stenDim->currentDim;
    tabDimSize = tabDim->currentDim;

    // Compute value shift on the table
    tabShiftVal = stenDimSize/2;

    // find adresses for the stencil applications on the table
    // to find the (i,j) pixel value
    currentDimensionElemIndice = tabIndices->component.tableDimensionsList.val->component.number;
    for(int i=0; i<stenDimSize; i++)
    {
      temp = codegen_init();
      // First case, the aimed value indice in the current dimension is less than 0
      if((currentDimensionElemIndice - tabShiftVal + i) < 0)
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim, tabSym, stenSym, tabLastBlockAdr, stenLastBlockAdr, symbol_table);
      }
      // Second case, the aimed value indice in the current dimension is greater than the table current dimension size
      else if((currentDimensionElemIndice - tabShiftVal + i) > (tabDim->currentDim -1))
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr  + ((tabDim->currentDim - 1) * tabDimSizesMultiplication), stenLastBlockAdr, symbol_table);
      }
      // Third case, the aimed value indice in the current dimension is in the current dimension
      else
      {
        codegen_ast_stencilOperationAux(temp, tabIndices->component.tableDimensionsList.nextDim, tabDim->nextDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr  + ((currentDimensionElemIndice - tabShiftVal + i) * tabDimSizesMultiplication), stenLastBlockAdr, symbol_table);
      }

      // Concat producted quads

      temporaryVar = cg->result;
      cg->code = concat(cg->code, temp->code);

      if(i>0)
      {
        val0.integer = 0;
        temporaryVarBis = symTable_newTemp(symbol_table, INT, val0);
        quad_add(cg->code, AST_OP_ADD, temporaryVar, temp->result, temporaryVarBis);
        cg->result = temporaryVarBis;
      }
      else
      {
        cg->result = temp->result;
      }


      codegen_keepQuadList_free(temp);
    }
  }
  // Third case : stencil has  a biger amount of dimensions than table
  else if((stenDim->nextDim != NULL) && (tabDim->nextDim == NULL))
  {
    tempStenDims = stenDim->nextDim;

    // Compute stenDimSizesMultiplication
    while(tempStenDims != NULL)
    {
      stenDimSizesMultiplication *= tempStenDims->currentDim;
      tempStenDims = tempStenDims->nextDim;
    }

    // Find table and stencil current dimension size
    stenDimSize = stenDim->currentDim;
    tabDimSize = tabDim->currentDim;

    // Compute value shift on the table
    tabShiftVal = stenDimSize/2;

    // find adresses for the stencil applications on the table
    // to find the (i,j) pixel value
    currentDimensionElemIndice = tabIndices->component.tableDimensionsList.val->component.number;
    for(int i=0; i<stenDimSize; i++)
    {
      temp = codegen_init();
      // First case, the aimed value indice in the current dimension is less than 0
      if((currentDimensionElemIndice - tabShiftVal + i) < 0)
      {
        codegen_ast_stencilOperationAux(temp, tabIndices, tabDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr, stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }
      // Second case, the aimed value indice in the current dimension is greater than the table current dimension size
      else if((currentDimensionElemIndice - tabShiftVal + i) > (tabDim->currentDim -1))
      {
        codegen_ast_stencilOperationAux(temp, tabIndices, tabDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr, stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }
      // Third case, the aimed value indice in the current dimension is in the current dimension
      else
      {
        codegen_ast_stencilOperationAux(temp, tabIndices, tabDim, stenDim->nextDim, tabSym, stenSym, tabLastBlockAdr, stenLastBlockAdr + (i * stenDimSizesMultiplication), symbol_table);
      }

      // Concat producted quads

      temporaryVar = cg->result;
      cg->code = concat(cg->code, temp->code);

      if(i>0)
      {
        val0.integer = 0;
        temporaryVarBis = symTable_newTemp(symbol_table, INT, val0);
        quad_add(cg->code, AST_OP_ADD, temporaryVar, temp->result, temporaryVarBis);
        cg->result = temporaryVarBis;
      }
      else
      {
        cg->result = temp->result;
      }

      codegen_keepQuadList_free(temp);
    }
  }
  // end of recursion, we are at the last dimension of stencil AND table
  else if((stenDim->nextDim == NULL) && (tabDim->nextDim == NULL))
  {
    // Find table and stencil current dimension size
    stenDimSize = stenDim->currentDim;
    tabDimSize = tabDim->currentDim;

    // Compute value shift on the table
    tabShiftVal = stenDimSize/2;

    // find adresses for the stencil applications on the table
    // to find the (i,j) pixel value
    currentDimensionElemIndice = tabIndices->component.tableDimensionsList.val->component.number;
    for(int i=0; i<stenDimSize; i++)
    {
      // First case, the aimed value indice in the current dimension is less than 0
      if((currentDimensionElemIndice - tabShiftVal + i) < 0)
      {
        // Generate adr + tab name for stencil
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", (stenLastBlockAdr + i) * WORDSIZE, stenSym->identifier);
        val.string = tabAcessForMIPSUsage;
        stenElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);

        // Generate adr + tab name for table
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", tabLastBlockAdr * WORDSIZE, tabSym->identifier);
        val.string = tabAcessForMIPSUsage;
        tabElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);
      }
      // Second case, the aimed value indice in the current dimension is greater than the table current dimension size
      else if((currentDimensionElemIndice - tabShiftVal + i) > (tabDim->currentDim -1))
      {
        // Generate adr + tab name for stencil
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", (stenLastBlockAdr + i) * WORDSIZE, stenSym->identifier);
        val.string = tabAcessForMIPSUsage;
        stenElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);

        // Generate adr + tab name for table
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", (tabLastBlockAdr + (tabDimSize-1)) * WORDSIZE, tabSym->identifier);
        val.string = tabAcessForMIPSUsage;
        tabElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);
      }
      // Third case, the aimed value indice in the current dimension is in the current dimension
      else
      {
        // Generate adr + tab name for stencil
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", (stenLastBlockAdr + i) * WORDSIZE, stenSym->identifier);
        val.string = tabAcessForMIPSUsage;
        stenElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);

        // Generate adr + tab name for table
        tabAcessForMIPSUsage = calloc(256, sizeof(char));
        snprintf(tabAcessForMIPSUsage, 256, "%d %s", (tabLastBlockAdr + (currentDimensionElemIndice - tabShiftVal + i)) * WORDSIZE, tabSym->identifier);
        val.string = tabAcessForMIPSUsage;
        tabElemAdr = symTable_addTabElemAdr(symbol_table, STRING, val);
      }

      // quads creations

      val0.integer = 0;
      temporaryVar = symTable_newTemp(symbol_table, INT, val0);

      if(i>0)
      {
        val0.integer = 0;
        temporaryVarBis = symTable_newTemp(symbol_table, INT, val0);
        quad_add(cg->code, AST_OP_MULT, stenElemAdr, tabElemAdr, temporaryVar);
        quad_add(cg->code, AST_OP_ADD, cg->result, temporaryVar, temporaryVarBis);
        cg->result = temporaryVarBis;
      }
      else
      {
        quad_add(cg->code, AST_OP_MULT, stenElemAdr, tabElemAdr, temporaryVar);
        cg->result = temporaryVar;
      }
    }
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

        break;

      case AST_OP_INCR:
        left = codegen_ast(left, ast->component.operation.left, symbol_table);
        quadList_free_keepList(right->code);

        codegen_ast_operations(cg, AST_OP_INCR, left, NULL, symbol_table);
        break;

      case AST_OP_DECR:
        left = codegen_ast(left, ast->component.operation.left, symbol_table);
        quadList_free_keepList(right->code);

        codegen_ast_operations(cg, AST_OP_DECR, left, NULL, symbol_table);
        break;

      case AST_RETURN:
        left = codegen_ast(left, ast->component.operation.left, symbol_table);
        quadList_free_keepList(right->code);

        codegen_ast_operations(cg, AST_RETURN, left, NULL, symbol_table);
        break;

      case AST_OP_STEN:
        codegen_ast_stencilOperation(cg, ast, symbol_table);
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

      // Control structure
      case AST_BOOL_TREE :
        break;

      case AST_IF :
        codegen_ast_controlStructure(cg, left, right, ast, symbol_table);
        break;

      case AST_WHILE :
        codegen_ast_controlStructure(cg, left, right, ast, symbol_table);
        break;

      case AST_FOR  :
        codegen_ast_controlStructure(cg, left, right, ast, symbol_table);
        break;

      // Table
      case AST_TAB_DECL :
        codegen_ast_tableDeclaration(cg, ast, symbol_table);
        quadList_free_keepList(right->code);
        quadList_free_keepList(left->code);
        break;

      case AST_TAB_ACSS :
        codegen_ast_tableAccess(cg, ast, symbol_table);
        quadList_free_keepList(right->code);
        quadList_free_keepList(left->code);
        break;

      case AST_STENCIL_DECL :
        codegen_ast_tableDeclaration(cg, ast, symbol_table);
        quadList_free_keepList(right->code);
        quadList_free_keepList(left->code);
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
