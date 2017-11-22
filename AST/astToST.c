#include "astToST.h"
#include <stdlib.h>
#include <stdio.h>

ast* genSymTable_ast(ast* tree, symTable* st)
{
  switch(tree->type)
  {
    // leafs
    case AST_INT     :  break;
    case AST_ID      :  genSymTable_const(tree, st);
                        break;
    // Binary operations
    case AST_OP_ADD  :  genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;
    case AST_OP_SUB  :  genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;
    case AST_OP_MULT :  genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;
    case AST_OP_DIV :   genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;
    // Unary operations
    case AST_OP_INCR :  genSymTable_ast(tree->component.operation.left, st);
                        break;
    case AST_OP_DECR :  genSymTable_ast(tree->component.operation.left, st);
                        break;
    case AST_OP_MINUS : genSymTable_ast(tree->component.operation.left, st);
                        break;
    // Function
    case AST_FUNC_DEF : genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;
    case AST_FUNC_CALL: genSymTable_ast(tree->component.operation.left, st);
                        genSymTable_ast(tree->component.operation.right, st);
                        break;

    default         :   break;
  }
  return tree;
}


symbol* genSymTable_newTemp(ast* tree, symTable* st)
{
  return symTable_newTemp(st, tree->component.number);
}

symbol* genSymTable_const(ast* tree, symTable* st)
{
  return symTable_addConst(st, tree->component.identifier);
}
