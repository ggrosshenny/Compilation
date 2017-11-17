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
      default         :   break;
    }
  }
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
    case AST_OP_DIV :   ast_free(tree->component.operation.left);
                        ast_free(tree->component.operation.right);
                        free(tree);
                        break;
    // Unary operations
    case AST_OP_INCR :  ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    case AST_OP_DECR :  ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    case AST_OP_MINUS : ast_free(tree->component.operation.left);
                        free(tree);
                        break;
    // Function
    case AST_FUNC_DEF : free(tree->component.function.identifier);
                        ast_free(tree->component.function.arguments);
                        ast_free(tree->component.function.body);
                        free(tree);
                        break;
    case AST_FUNC_CALL: free(tree->component.function.identifier);
                        ast_free(tree->component.function.arguments);
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
  ast* newAst = malloc(sizeof(ast));
  newAst->type = type;
  newAst->component.operation.left = left0;
  newAst->component.operation.right = right0;
  return newAst;
}


ast* ast_new_unaryOperation(enum ast_type type, ast* operand)
{
  ast* newAst = malloc(sizeof(ast));
  newAst->type = type;
  newAst->component.operation.left = operand;
  newAst->component.operation.right = NULL;
  return newAst;
}

// Functions

ast* ast_new_functionDefinition(ast* id, ast* arguments, ast* functionBody)
{
  ast* newAst = malloc(sizeof(ast));
  newAst->type = AST_FUNC_DEF;
  newAst->component.function.identifier = id;
  newAst->component.function.arguments = arguments;
  newAst->component.function.body = functionBody;
  return newAst;
}


ast* ast_new_functionCall(ast* id, ast* arguments)
{
  ast* newAst = malloc(sizeof(ast));
  newAst->type = AST_FUNC_CALL;
  newAst->component.function.identifier = id;
  newAst->component.function.arguments = arguments;
  newAst->component.function.body = NULL;
  return newAst;
}


// Variables and integers


ast* ast_new_number(int value)
{
  ast* newAst = malloc(sizeof(ast));
  newAst->type = AST_INT;
  newAst->component.number = value;
  return newAst;
}


ast* ast_new_identifier(char* id)
{
  ast* newAst = malloc(sizeof(ast));
  newAst->type = AST_ID;
  newAst->component.identifier = strndup(id, MAX_IDENTIFIER_LENGHT);
  return newAst;
}
