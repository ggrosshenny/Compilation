#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


// Auxiliary methods

void print_ast(ast* tree, int indent)
{
  if(tree != NULL)
  {
    for(int i=0; i<indent; i++)
    {
      printf("\t");
    }

    switch(tree->type)
    {
      // leafs
      case AST_INT     :  printf("%d\n", tree->component.number);
                          break;
      case AST_STR     :  printf("%s\n", tree->component.string);
                          break;
      case AST_ID      :  printf("%s\n", tree->component.identifier);
                          break;
      // Binary operations
      case AST_OP_ADD  :  indent++;
                          printf("Addition\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      case AST_OP_SUB  :  indent++;
                          printf("Substraction\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      case AST_OP_MULT :  indent++;
                          printf("Multiplication\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      case AST_OP_DIV :   indent++;
                          printf("Division\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      case AST_OP_AFCT  : indent++;
                          printf("Affectation\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      case AST_OP_DECL  : indent++;
                          printf("Declaration\n");
                          print_ast(tree->component.operation.left, indent);
                          print_ast(tree->component.operation.right, indent);
                          break;
      // Unary operations
      case AST_OP_INCR :  indent++;
                          printf("Incrementation\n");
                          print_ast(tree->component.operation.left, indent);
                          break;
      case AST_OP_DECR :  indent++;
                          printf("Decrementation\n");
                          print_ast(tree->component.operation.left, indent);
                          break;
      case AST_OP_MINUS : indent++;
                          printf("Minus\n");
                          print_ast(tree->component.operation.left, indent);
                          break;
      // Boolean expressions
      case AST_BOOL_TREE: indent++;
                          printf("Boolean tree\n");
                          print_ast(tree->component.boolean.boolExpr, indent);
                          print_ast(tree->component.boolean.ast_true, indent);
                          print_ast(tree->component.boolean.ast_false, indent);
                          break;
      case AST_BOOL_EQ   :  indent++;
                            printf("Equal\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_BOOL_NEQ  :  indent++;
                            printf("Not equal\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_BOOL_GT    : indent++;
                            printf("Greater than\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_BOOL_GEQ   : indent++;
                            printf("Greater equal\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_BOOL_LT    : indent++;
                            printf("Less than\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_BOOL_LEQ   : indent++;
                            printf("Less equal\n");
                            print_ast(tree->component.operation.left, indent);
                            print_ast(tree->component.operation.right, indent);
                            break;
      case AST_GOTO       : indent++;
                            printf("GOTO\n");
                            print_ast(tree->component.operation.left, indent);
                            break;
      case AST_IF         : indent++;
                            printf("control structure IF\n");
                            print_ast(tree->component.boolean.boolExpr, indent);
                            print_ast(tree->component.boolean.ast_true, indent);
                            print_ast(tree->component.boolean.ast_false, indent);
                            break;
      case AST_WHILE      : indent++;
                            printf("control structure WHILE\n");
                            print_ast(tree->component.boolean.boolExpr, indent);
                            print_ast(tree->component.boolean.ast_true, indent);
                            print_ast(tree->component.boolean.ast_false, indent);
                            break;
      case AST_FOR        : indent++;
                            printf("control structure FOR\n");
                            print_ast(tree->component.boolean.boolExpr, indent);
                            print_ast(tree->component.boolean.ast_true, indent);
                            print_ast(tree->component.boolean.ast_false, indent);
                            print_ast(tree->component.boolean.forVarInit, indent);
                            print_ast(tree->component.boolean.forVarUpdate, indent);
                            break;
      // Functions
      case AST_FUNC_DEF  :  indent++;
                            printf("Function definition\n");
                            print_ast(tree->component.function.identifier, indent);
                            print_ast(tree->component.function.arguments, indent);
                            print_ast(tree->component.function.body, indent);
                            break;
      case AST_FUNC_ARG  :  indent++;
                            printf("Argument\n");
                            print_ast(tree->component.argumentsList.argument, indent);
                            print_ast(tree->component.argumentsList.nextArg, --indent);
                            break;
      case AST_FUNC_CALL :  indent++;
                            printf("Function call\n");
                            print_ast(tree->component.function.identifier, indent);
                            print_ast(tree->component.function.arguments, indent);
                            break;
      case AST_FUNC_BODY :  indent++;
                            printf("Instruction\n");
                            print_ast(tree->component.instructionsList.instruction, indent);
                            print_ast(tree->component.instructionsList.nextInstruction, --indent);
                            break;
      // Table
      case AST_TAB_DECL : indent++;
                          printf("Table declaration\n");
                          print_ast(tree->component.tableDeclaration.identifier, indent);
                          print_ast(tree->component.tableDeclaration.dimensions, indent);
                          print_ast(tree->component.tableDeclaration.elements, indent);
                          break;
      case AST_TAB_ACSS : indent++;
                          printf("Table access\n");
                          print_ast(tree->component.tableAccess.identifier, indent);
                          print_ast(tree->component.tableAccess.indices, indent);
                          break;
      case AST_TAB_DIM :  indent++;
                          printf("dimension (size or indice)\n");
                          print_ast(tree->component.tableDimensionsList.val, indent);
                          print_ast(tree->component.tableDimensionsList.nextDim, --indent);
                          break;
      case AST_TAB_BLCK : indent++;
                          printf("elements block\n");
                          print_ast(tree->component.tableElementsBlock.elements, indent);
                          print_ast(tree->component.tableElementsBlock.nextBlock, --indent);
                          break;
      case AST_TAB_ELEM:  indent++;
                          printf("Element\n");
                          print_ast(tree->component.tableElementsList.currentElem, indent);
                          print_ast(tree->component.tableElementsList.nextElem, --indent);
                          break;

      default         :   break;
    }
  }
}


ast* ast_concat(ast* mainAST, ast* astToAdd)
{
  if(astToAdd == NULL)
  {
    return mainAST;
  }
  // Verification
  if(mainAST->type != astToAdd->type)
  {
    ast_free(mainAST);
    ast_free(astToAdd);
    fprintf(stderr, "ERROR : You're trying to concat two AST with different types !\n");
    exit(1);
  }

  ast* temp = NULL;

  switch(mainAST->type)
  {
    case AST_FUNC_BODY :
      temp = mainAST;
      while(temp->component.instructionsList.nextInstruction != NULL)
      {
        temp = temp->component.instructionsList.nextInstruction;
      }
      temp->component.instructionsList.nextInstruction = astToAdd;
      break;

    case AST_FUNC_ARG  :
      temp = mainAST;
      while(temp->component.argumentsList.nextArg != NULL)
      {
        temp = temp->component.argumentsList.nextArg;
      }
      temp->component.argumentsList.nextArg = astToAdd;
      break;

    case AST_TAB_ELEM :
      temp = mainAST;
      while(temp->component.tableElementsList.nextElem != NULL)
      {
        temp = temp->component.tableElementsList.nextElem;
      }
      temp->component.tableElementsList.nextElem = astToAdd;
      break;

    case AST_TAB_DIM :
      temp = mainAST;
      while(temp->component.tableDimensionsList.nextDim != NULL)
      {
        temp = temp->component.tableDimensionsList.nextDim;
      }
      temp->component.tableDimensionsList.nextDim = astToAdd;
      break;

    case AST_TAB_BLCK :
      temp = mainAST;
      while(temp->component.tableElementsBlock.nextBlock != NULL)
      {
        temp = temp->component.tableElementsBlock.nextBlock;
      }
      temp->component.tableElementsBlock.nextBlock = astToAdd;
      break;

    default:
      ast_free(mainAST);
      ast_free(astToAdd);
      fprintf(stderr, "ERROR : You're trying to concat two AST with invalid types !\n");
      exit(1);
      break;
  }

  return mainAST;
}


// ========================
// Memory release functions

void ast_free(ast* tree)
{
  if(tree == NULL)
  {
    return;
  }

  switch(tree->type)
  {
    // leafs
    case AST_INT     :  free(tree);
                        break;
    case AST_STR     :  free(tree->component.string);
                        free(tree);
                        break;
    case AST_ID      :  free(tree->component.identifier);
                        free(tree);
                        break;
    // Binary operations
    case AST_OP_ADD  :  ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_OP_SUB  :  ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_OP_MULT :  ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_OP_DIV  :  ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_OP_AFCT  : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_OP_DECL  : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    // Unary operations
    case AST_OP_INCR  : ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    case AST_OP_DECR  : ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    case AST_OP_MINUS : ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    // Boolean expression
    case AST_BOOL_TREE: ast_free(tree->component.boolean.boolExpr);
                        ast_free(tree->component.boolean.ast_true);
                        ast_free(tree->component.boolean.ast_false);
                        free(tree);
                        break;
    case AST_BOOL_EQ  : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_BOOL_NEQ : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_BOOL_GT  : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_BOOL_GEQ : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_BOOL_LT  : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    case AST_BOOL_LEQ : ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    // Control structures
    case AST_GOTO     : free(tree);
                        break;
    case AST_IF       : ast_free(tree->component.boolean.boolExpr);
                        ast_free(tree->component.boolean.ast_true);
                        ast_free(tree->component.boolean.ast_false);
                        free(tree);
                        break;
    case AST_WHILE    : ast_free(tree->component.boolean.boolExpr);
                        ast_free(tree->component.boolean.ast_true);
                        ast_free(tree->component.boolean.ast_false);
                        free(tree);
                        break;
    case AST_FOR      : ast_free(tree->component.boolean.boolExpr);
                        ast_free(tree->component.boolean.ast_true);
                        ast_free(tree->component.boolean.ast_false);
                        ast_free(tree->component.boolean.forVarInit);
                        ast_free(tree->component.boolean.forVarUpdate);
                        free(tree);
                        break;
    // Function
    case AST_FUNC_DEF : ast_free(tree->component.function.arguments);
                        ast_free(tree->component.function.body);
                        ast_free(tree->component.function.identifier);
                        free(tree);
                        break;
    case AST_FUNC_CALL: ast_free(tree->component.function.arguments);
                        ast_free(tree->component.function.identifier);
                        free(tree);
                        break;
    case AST_FUNC_BODY: ast_free(tree->component.instructionsList.instruction);
                        ast_free(tree->component.instructionsList.nextInstruction);
                        free(tree);
                        break;
    case AST_FUNC_ARG:  ast_free(tree->component.argumentsList.argument);
                        ast_free(tree->component.argumentsList.nextArg);
                        free(tree);
                        break;
    // Table
    case AST_TAB_DECL:  ast_free(tree->component.tableDeclaration.identifier);
                        ast_free(tree->component.tableDeclaration.dimensions);
                        ast_free(tree->component.tableDeclaration.elements);
                        free(tree);
                        break;
    case AST_TAB_ACSS:  ast_free(tree->component.tableAccess.identifier);
                        ast_free(tree->component.tableAccess.indices);
                        free(tree);
                        break;
    case AST_TAB_DIM:   ast_free(tree->component.tableDimensionsList.nextDim);
                        ast_free(tree->component.tableDimensionsList.val);
                        free(tree);
                        break;
    case AST_TAB_ELEM:  ast_free(tree->component.tableElementsList.nextElem);
                        ast_free(tree->component.tableElementsList.currentElem);
                        free(tree);
                        break;
    case AST_TAB_BLCK:  ast_free(tree->component.tableElementsBlock.elements);
                        ast_free(tree->component.tableElementsBlock.nextBlock);
                        free(tree);
                        break;

    default         :   break;
  }
}



// ===========================
// Memory allocation functions

// Operations definition

ast* ast_new_binaryOperation(enum ast_type type, ast* left0, ast* right0)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = type;
  newAst->component.operation.left = left0;
  newAst->component.operation.right = right0;
  return newAst;
}


ast* ast_new_unaryOperation(enum ast_type type, ast* operand)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = type;
  newAst->component.operation.left = operand;
  newAst->component.operation.right = NULL;
  return newAst;
}

// Functions

ast* ast_new_functionDefinition(ast* id, ast* arguments, ast* functionBody)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_FUNC_DEF;
  newAst->component.function.identifier = id;
  newAst->component.function.arguments = arguments;
  newAst->component.function.body = functionBody;
  return newAst;
}


ast* ast_new_functionCall(ast* id, ast* arguments)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_FUNC_CALL;
  newAst->component.function.identifier = id;
  newAst->component.function.arguments = arguments;
  newAst->component.function.body = NULL;
  return newAst;
}


ast* ast_new_Instruction(ast* instruction0)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_FUNC_BODY;
  newAst->component.instructionsList.instruction = instruction0;
  newAst->component.instructionsList.nextInstruction = NULL;
  return newAst;
}

ast* ast_new_argument(ast* argument0)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_FUNC_ARG;
  newAst->component.argumentsList.argument = argument0;
  newAst->component.argumentsList.nextArg = NULL;
  return newAst;
}


// Boolean expressions

void placeGoto(ast* boolTree0, ast* true0, ast* false0){

  if(boolTree0->component.boolean.ast_true == NULL
     && boolTree0->component.boolean.ast_false == NULL){

    boolTree0->component.boolean.ast_true = ast_new_unaryOperation(AST_GOTO, true0);
    boolTree0->component.boolean.ast_false = ast_new_unaryOperation(AST_GOTO, false0);
  }

  else{

    if(boolTree0->component.boolean.ast_true != NULL){
      placeGoto(boolTree0->component.boolean.ast_true, true0, false0);
    }
    else{
      boolTree0->component.boolean.ast_true = ast_new_unaryOperation(AST_GOTO, true0);
    }

    if(boolTree0->component.boolean.ast_false != NULL){
      placeGoto(boolTree0->component.boolean.ast_false, true0, false0);
    }
    else{
      boolTree0->component.boolean.ast_false = ast_new_unaryOperation(AST_GOTO, false0);
    }

  }
}


ast* ast_new_controlStructure(enum ast_type type, ast* conditionsList0, ast* true0, ast* false0, ast* varInit0, ast* varUpdate0){
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = type;
  newAst->component.boolean.boolExpr = conditionsList0;
  newAst->component.boolean.ast_true = true0;
  newAst->component.boolean.ast_false = false0;

  if(type == AST_FOR){
    newAst->component.boolean.forVarInit = varInit0;
    newAst->component.boolean.forVarUpdate = varUpdate0;
  }

  return newAst;
}


ast* ast_new_boolExpr(ast* boolExpr0, ast* ast_true0, ast* ast_false0){
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_BOOL_TREE;
  newAst->component.boolean.boolExpr = boolExpr0;
  newAst->component.boolean.ast_true = ast_true0;
  newAst->component.boolean.ast_false = ast_false0;
  return newAst;
}


// Table


ast* ast_new_tabDeclaration(ast* identifier0, ast* dimensions0, ast* elements0)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_TAB_DECL;
  newAst->component.tableDeclaration.identifier = identifier0;
  newAst->component.tableDeclaration.dimensions = dimensions0;
  newAst->component.tableDeclaration.elements = elements0;
  return newAst;
}


ast* ast_new_tableAccess(ast* identifier0, ast* indices0)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_TAB_ACSS;
  newAst->component.tableDeclaration.identifier = identifier0;
  newAst->component.tableDeclaration.dimensions = indices0;
  return newAst;
}


ast* ast_new_tabDimension(ast* valeur)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_TAB_DIM;
  newAst->component.tableDimensionsList.val = valeur;
  newAst->component.tableDimensionsList.nextDim = NULL;
  return newAst;
}


ast* ast_new_tabElements(ast* element)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_TAB_ELEM;
  newAst->component.tableElementsList.currentElem = element;
  newAst->component.tableElementsList.nextElem = NULL;
  return newAst;
}


ast* ast_new_tableElementsBlock(ast* block)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_TAB_BLCK;
  newAst->component.tableElementsBlock.elements = block;
  newAst->component.tableElementsBlock.nextBlock = NULL;
  return newAst;
}


// Variables and integers


ast* ast_new_number(int value)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_INT;
  newAst->component.number = value;
  return newAst;
}


ast* ast_new_string(char* str)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_STR;
  newAst->component.string = strndup(str, MAX_STR_LENGHT);
  return newAst;
}


ast* ast_new_identifier(char* id)
{
  ast* newAst = calloc(1, sizeof(ast));
  newAst->type = AST_ID;
  newAst->component.identifier = strndup(id, MAX_IDENTIFIER_LENGHT);
  return newAst;
}
