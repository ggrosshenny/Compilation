#include "astToST.h"
#include <stdlib.h>
#include <stdio.h>


// Auxiliary functions

void genSymTable_error(char* message, ast* tree, symTable* st)
{
  ast_free(tree);
  symTable_free(st);
  fprintf(stderr, "%s\n", message);
  exit(1);
}



// Main functions


symTable* genSymTable_init(ast* tree)
{
  symTable* st = symTable_init(tree);

  ast* printf_ast_argID = ast_new_identifier("msg");
  ast* printf_ast_ID = ast_new_identifier("printf");
  ast* printf_arg_decl = ast_new_binaryOperation(AST_OP_DECL, printf_ast_argID, NULL);
  ast* printf_ast_arg = ast_new_argument(printf_arg_decl);
  ast* printf_ast = ast_new_functionDefinition(printf_ast_ID, printf_ast_arg, NULL);
  genSymTable_functionDeclaration(printf_ast, st);
  ast_free(printf_ast);

  ast* printi_ast_argID = ast_new_identifier("msg");
  ast* printi_ast_ID = ast_new_identifier("printi");
  ast* printi_arg_decl = ast_new_binaryOperation(AST_OP_DECL, printi_ast_argID, NULL);
  ast* printi_ast_arg = ast_new_argument(printi_arg_decl);
  ast* printi_ast = ast_new_functionDefinition(printi_ast_ID, printi_ast_arg, NULL);
  genSymTable_functionDeclaration(printi_ast, st);
  ast_free(printi_ast);

  genSymTable_ast(tree, st);

  return st;
}


ast* genSymTable_ast(ast* tree, symTable* st)
{
  if(tree != NULL)
  {
    switch(tree->type)
    {
      // leafs
      case AST_INT :
        break;
      case AST_STR :
        break;
      case AST_ID :
        genSymTable_const(tree, st);
        break;

      // Binary operations
      case AST_OP_ADD :
        genSymTable_binaryOperation(tree, st);
        break;
      case AST_OP_SUB :
        genSymTable_binaryOperation(tree, st);
        break;
      case AST_OP_MULT :
        genSymTable_binaryOperation(tree, st);
        break;
      case AST_OP_DIV :
        genSymTable_binaryOperation(tree, st);
        break;
      case AST_OP_DECL :
        genSymTable_declaration(tree, st);
        break;
      case AST_OP_AFCT :
        genSymTable_binaryOperation(tree, st);
        break;

      // Unary operations
      case AST_OP_INCR :
        genSymTable_unaryOperation(tree, st);
        break;
      case AST_OP_DECR :
        genSymTable_unaryOperation(tree, st);
        break;
      case AST_OP_MINUS :
        genSymTable_unaryOperation(tree, st);
        break;

      // Function
      case AST_FUNC_DEF :
        genSymTable_functionDeclaration(tree, st);
        break;
      case AST_FUNC_ARG :
        genSymTable_ast(tree->component.argumentsList.argument, st);
        genSymTable_ast(tree->component.argumentsList.nextArg, st);
        break;
      case AST_FUNC_CALL :
        genSymTable_ast(tree->component.argumentsList.argument, st);
        genSymTable_ast(tree->component.argumentsList.nextArg, st);
        break;
      case AST_FUNC_BODY :
        genSymTable_ast(tree->component.instructionsList.instruction, st);
        genSymTable_ast(tree->component.instructionsList.nextInstruction, st);
        break;

      // Control structure
      case AST_IF :
        genSymTable_ast(tree->component.boolean.boolExpr, st);
        genSymTable_ast(tree->component.boolean.ast_true, st);
        genSymTable_ast(tree->component.boolean.ast_false, st);
        break;
      case AST_GOTO :
        genSymTable_ast(tree->component.operation.left, st);
      case AST_BOOL_TREE :
        genSymTable_ast(tree->component.boolean.boolExpr, st);
        genSymTable_ast(tree->component.boolean.ast_true, st);
        genSymTable_ast(tree->component.boolean.ast_false, st);
        break;
        // Binary boolean operations
      case AST_BOOL_EQ :
        genSymTable_binaryOperation(tree, st);
      case AST_BOOL_NEQ :
        genSymTable_binaryOperation(tree, st);
      case AST_BOOL_GEQ :
        genSymTable_binaryOperation(tree, st);
      case AST_BOOL_LEQ :
        genSymTable_binaryOperation(tree, st);
      case AST_BOOL_GT :
        genSymTable_binaryOperation(tree, st);
      case AST_BOOL_LT :
        genSymTable_binaryOperation(tree, st);

      default         :   break;
    }
  }
  return tree;
}


ast* genSymTable_binaryOperation(ast* tree, symTable* st)
{
  ast* left = tree->component.operation.left;
  ast* right = tree->component.operation.right;
  if(left->type == AST_ID)
  {
    if(symTable_lookUp(st, left->component.identifier) == NULL)
    {
      char msg[MAX_IDENTIFIER_LENGHT+250];
      snprintf(msg, MAX_IDENTIFIER_LENGHT+250, "Compilation error : Identifier %s was not declared before use !\n", left->component.identifier);
      genSymTable_error(msg, st->tree, st);
    }
  }
  if(right->type == AST_ID)
  {
    if(symTable_lookUp(st, right->component.identifier) == NULL)
    {
      char msg[MAX_IDENTIFIER_LENGHT+250];
      snprintf(msg, MAX_IDENTIFIER_LENGHT+250, "Compilation error : Identifier %s was not declared before use !\n", right->component.identifier);
      genSymTable_error(msg, st->tree, st);
    }
  }
  genSymTable_ast(left, st);
  genSymTable_ast(right, st);

  return tree;
}


ast* genSymTable_unaryOperation(ast* tree, symTable* st)
{
  ast* left = tree->component.operation.left;

  if(left->type)
  {
    if(symTable_lookUp(st, left->component.identifier) == NULL)
    {
      char msg[MAX_IDENTIFIER_LENGHT+250];
      snprintf(msg, MAX_IDENTIFIER_LENGHT+250, "Compilation error : Identifier %s was not declared before use !\n", left->component.identifier);
      genSymTable_error(msg, st->tree, st);
    }
  }

  genSymTable_ast(left, st);

  return tree;
}


ast* genSymTable_declaration(ast* tree, symTable* st)
{
  ast* left = tree->component.operation.left;
  ast* right = tree->component.operation.right;

  left = genSymTable_ast(left, st);
  right = genSymTable_ast(right, st);

  return tree;
}


ast* genSymTable_functionDeclaration(ast* tree, symTable* st)
{
  ast* funcIndentifier = tree->component.function.identifier;
  ast* functionArgument = NULL;
  args* newArg = NULL;
  args* lastArg = NULL;
  char* argName;
  int nb_arguments = 0;

  genSymTable_ast(funcIndentifier, st);
  symbol* tempId = symTable_lookUp(st, funcIndentifier->component.identifier);
  tempId->isFunction = true;
  tempId->isConstant = false;
  tempId->isVoidFunction = true;
  genSymTable_ast(tree->component.function.arguments, st);
  genSymTable_ast(tree->component.function.body, st);

  if(tree->component.function.arguments != NULL)
  {
    functionArgument = tree->component.function.arguments;

    // Initialize the list of arguments of the function
    argName = functionArgument->component.argumentsList.argument->component.operation.left->component.identifier;
    symbol* testArg = symTable_lookUp(st, argName);
    newArg = genSymTable_aux_newArg(testArg);
    tempId->content.val.arguments = newArg;
    nb_arguments++;

    while((functionArgument = functionArgument->component.argumentsList.nextArg) != NULL)
    {
      // construire liste chainée d'arguments
      lastArg = newArg;
      argName = functionArgument->component.argumentsList.argument->component.operation.left->component.identifier;

      newArg = genSymTable_aux_newArg(symTable_lookUp(st, argName));
      nb_arguments++;

      lastArg->nextArg = newArg;
    }
  }

  if(nb_arguments > 0)
  {
    tempId->isVoidFunction = false;
  }

  return tree;
}

args* genSymTable_aux_newArg(symbol* smbl)
{
  args* newArg = calloc(1, sizeof(args));
  newArg->currentArg = smbl;
  newArg->nextArg = NULL;
  return newArg;
}


ast* genSymTable_functionCall(ast* tree, symTable* st)
{
  ast* identifier = tree->component.function.identifier;

  if(symTable_lookUp(st, identifier->component.identifier) == NULL)
  {
    char msg[MAX_IDENTIFIER_LENGHT+250];
    snprintf(msg, MAX_IDENTIFIER_LENGHT+250, "Compilation error : Identifier %s was not declared before use !\n", identifier->component.identifier);
    genSymTable_error(msg, st->tree, st);
  }

  genSymTable_ast(identifier, st);
  genSymTable_ast(tree->component.function.arguments, st);
  genSymTable_ast(tree->component.function.body, st);

  return tree;
}


symbol* genSymTable_newTemp(ast* tree, symTable* st)
{
  value newVal;
  newVal.integer = tree->component.number;
  return symTable_newTemp(st, INT, newVal);
}

symbol* genSymTable_const(ast* tree, symTable* st)
{
  return symTable_addConst(st, tree->component.identifier);
}
