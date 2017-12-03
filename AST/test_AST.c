#include <string.h>
#include <assert.h>
#include "ast.h"

int main()
{
    ast* astTest = NULL;

    // Tests

    // Test declaration
    ast* testId = ast_new_identifier("Test");
    assert(testId->type == AST_ID);
    assert(strcmp(testId->component.identifier, "Test") == 0);

    ast* testVal = ast_new_number(25);
    assert(testVal->type == AST_INT);
    assert(testVal->component.number == 25);

    ast* testIdDeclaration = ast_new_binaryOperation(AST_OP_DECL, testId, NULL);
    ast* testIdAffectation = ast_new_binaryOperation(AST_OP_AFCT, testIdDeclaration, testVal);

    ast* instr0 = ast_new_Instruction(testIdAffectation);

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

    ast* instr1 = ast_new_Instruction(mult2);

    ast* id = ast_new_identifier("lol");
    ast* instr2 = ast_new_Instruction(id);
    ast* temp = ast_concat(instr0, instr2);

    // IF structure
    ast* idToTest = ast_new_identifier("y");
    ast* idToTest2 = ast_new_identifier("y");
    ast* idToTest3 = ast_new_identifier("y");
    ast* idToTest4 = ast_new_identifier("y");
    ast* idToTest5 = ast_new_identifier("y");
    ast* valToTest = ast_new_number(25);
    ast* afctToTest = ast_new_binaryOperation(AST_OP_AFCT, idToTest, valToTest);
    ast* afctInstr = ast_new_Instruction(afctToTest);
    ast* condNumber = ast_new_number(30);
    ast* condNumber2 = ast_new_number(30);
    ast* condTest = ast_new_binaryOperation(AST_BOOL_LT, idToTest2, condNumber);
    ast* condBoolExpr = ast_new_boolExpr(condTest, NULL, NULL);
    ast* condTest2 = ast_new_binaryOperation(AST_BOOL_EQ, idToTest3, condNumber2);
    ast* condBoolExpr2 = ast_new_boolExpr(condTest2, NULL, NULL);
    condBoolExpr->component.boolean.ast_false = condBoolExpr2;  // condTest OR condTest2
      // Instruction if true
    ast* trueIncr = ast_new_unaryOperation(AST_OP_INCR, idToTest4);
    ast* instrIfTrue = ast_new_Instruction(trueIncr);
      // Instruction if false
    ast* falseDecr = ast_new_unaryOperation(AST_OP_DECR, idToTest5);
    ast* instrIfFalse = ast_new_Instruction(falseDecr);
      // Completing the tree
    placeGoto(condBoolExpr, instrIfTrue, instrIfFalse);
    ast* ifStruct = ast_new_controlStructure(AST_IF, condBoolExpr, instrIfTrue, instrIfFalse);
    ast* ifToInstr = ast_new_Instruction(ifStruct);

    ast* arg1ID = ast_new_identifier("arg1");
    ast* arg2ID = ast_new_identifier("arg2");
    ast* arg1Decl = ast_new_binaryOperation(AST_OP_DECL, arg1ID, NULL);
    ast* arg2Decl = ast_new_binaryOperation(AST_OP_DECL, arg2ID, NULL);
    ast* arg1ARG = ast_new_argument(arg1Decl);
    ast* arg2ARG = ast_new_argument(arg2Decl);
    ast* tempArg = ast_concat(arg1ARG, arg2ARG);

    ast* funcID = ast_new_identifier("main");
    astTest = ast_new_functionDefinition(funcID, tempArg, ast_concat(ast_concat(temp, instr1), ast_concat(afctInstr,ifToInstr)));
    //astTest = ast_new_functionDefinition(funcID, tempArg, ast_concat(temp, instr1));
    print_ast(astTest, 0);
      printf("%d\n",sizeof(ast));
    ast_free(astTest);

    return 0;
}
