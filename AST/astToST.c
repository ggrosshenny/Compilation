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

ast* genSymTable_ast(ast* tree, symTable* st)
{
  if(tree != NULL)
  {
    switch(tree->type)
    {
      // leafs
      case AST_INT     :  break;
      case AST_STR     :  break;
      case AST_ID      :  genSymTable_const(tree, st);
                          break;
      // Binary operations
      case AST_OP_ADD  :  genSymTable_binaryOperation(tree, st);
                          break;
      case AST_OP_SUB  :  genSymTable_binaryOperation(tree, st);
                          break;
      case AST_OP_MULT :  genSymTable_binaryOperation(tree, st);
                          break;
      case AST_OP_DIV :   genSymTable_binaryOperation(tree, st);
                          break;
      case AST_OP_DECL :  genSymTable_declaration(tree, st);
                          break;
      case AST_OP_AFCT :  genSymTable_binaryOperation(tree, st);
                          break;
      // Unary operations
      case AST_OP_INCR :  genSymTable_unaryOperation(tree, st);
                          break;
      case AST_OP_DECR :  genSymTable_unaryOperation(tree, st);
                          break;
      case AST_OP_MINUS : genSymTable_unaryOperation(tree, st);
                          break;
      // Function
      case AST_FUNC_DEF : genSymTable_functionDeclaration(tree, st);
                          break;
      case AST_FUNC_CALL: // genSymTable_ast(tree->component.operation.left, st);
                          // genSymTable_ast(tree->component.operation.right, st);
                          break;
      case AST_FUNC_BODY: genSymTable_ast(tree->component.instructionsList.instruction, st);
                          genSymTable_ast(tree->component.instructionsList.nextInstruction, st);
                          break;


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
  ast* funcIndentier = tree->component.function.identifier;
  genSymTable_ast(funcIndentier, st);
  symbol* tempId = symTable_lookUp(st, funcIndentier->component.identifier);
  tempId->isFunction = true;
  genSymTable_ast(tree->component.function.arguments, st);
  genSymTable_ast(tree->component.function.body, st);

  return tree;
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
