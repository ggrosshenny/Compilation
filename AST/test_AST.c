#include <string.h>
#include <assert.h>
#include "ast.h"

int main()
{
    ast* astTest = NULL;

    // Tests
      // leftOp
    ast* leftOp = ast_new_identifier("Test");
    assert(leftOp->type == AST_ID);
    assert(strcmp(leftOp->component.identifier, "Test") == 0);
      // rightOp
    ast* rightOp = ast_new_number(42);
    assert(rightOp->type == AST_INT);
    assert(rightOp->component.number == 42);
      // add
    ast* add =  ast_new_binaryOperation(AST_OP_ADD, leftOp, rightOp);
    assert(add->type == AST_OP_ADD);
    assert(add->component.operation.left->type == AST_ID);
    assert(add->component.operation.right->type == AST_INT);
      // scal
    ast* scal = ast_new_number(10);
    assert(scal->type == AST_INT);
    assert(scal->component.number == 10);
      // scal2
    ast* scal2 = ast_new_number(2);
    assert(scal2->type == AST_INT);
    assert(scal2->component.number == 2);
      // minus
    ast* minus = ast_new_unaryOperation(AST_OP_MINUS, scal2);
    assert(minus->type == AST_OP_MINUS);
    assert(minus->component.operation.left->type == AST_INT);
    assert(minus->component.operation.right == NULL);
      // mult
    ast* mult = ast_new_binaryOperation(AST_OP_MULT, scal, add);
    assert(mult->type == AST_OP_MULT);
    assert(mult->component.operation.left->type == AST_INT);
    assert(mult->component.operation.right->type == AST_OP_ADD);
      // mult2
    ast* mult2 = ast_new_binaryOperation(AST_OP_MULT, minus, mult);
    assert(mult2->type == AST_OP_MULT);
    assert(mult2->component.operation.left->type == AST_OP_MINUS);
    assert(mult2->component.operation.right->type == AST_OP_MULT);

    astTest = mult2;

    print_ast(astTest, 0);

    ast_free(astTest);

    return 0;
}
