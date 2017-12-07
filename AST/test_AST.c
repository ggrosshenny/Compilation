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
    ast* ifStruct = ast_new_controlStructure(AST_IF, condBoolExpr, instrIfTrue, instrIfFalse, NULL, NULL);
    ast* ifToInstr = ast_new_Instruction(ifStruct);


    // WHILE struct
    // ast* whileId = ast_new_identifier("z");
    // ast* whileId_2 = ast_new_identifier("z");
    // ast* whileId_3 = ast_new_identifier("z");
    // ast* whileId_4 = ast_new_identifier("z");
    // ast* whileId2 = ast_new_identifier("z2");
    // ast* whileId2_2 = ast_new_identifier("z2");
    // ast* whileId2_3 = ast_new_identifier("z2");
    // ast* whileValTest = ast_new_number(10);
    // ast* whileValInitz = ast_new_number(0);
    // ast* whileValInitz2 = ast_new_number(5);
    //
    // ast* whileAfct1 = ast_new_binaryOperation(AST_OP_AFCT, whileId, whileValInitz);
    // ast* whileAfct2 = ast_new_binaryOperation(AST_OP_AFCT, whileId2, whileValInitz2);
    // ast* whileAfctInstr1 = ast_new_Instruction(whileAfct1);
    // ast* whileAfctInstr2 = ast_new_Instruction(whileAfct2);
    //
    // ast* whileCondTest = ast_new_binaryOperation(AST_BOOL_NEQ, whileId_2, whileValTest);
    // ast* whileCondBoolExpr = ast_new_boolExpr(whileCondTest, NULL, NULL);
    //
    // ast* whileTrueAfct = ast_new_binaryOperation(AST_OP_AFCT, whileId2_2, ast_new_binaryOperation(AST_OP_ADD, whileId_3, whileId2_3));
    // ast* whileTrueIncr = ast_new_unaryOperation(AST_OP_INCR, whileId_4);
    // ast* whileInstrTrueAfct = ast_new_Instruction(whileTrueAfct);
    // ast* whileInstrTrueIncr = ast_new_Instruction(whileTrueIncr);
    // ast* whileTrueInstr = ast_concat(whileInstrTrueAfct, whileInstrTrueIncr);
    //
    // placeGoto(whileCondBoolExpr, whileTrueInstr, NULL);
    // ast* whileStruct = ast_new_controlStructure(AST_WHILE, whileCondBoolExpr, whileTrueInstr, NULL, NULL, NULL);
    // ast* whileInstr = ast_concat(whileAfctInstr1, ast_concat(whileAfctInstr2, ast_new_Instruction(whileStruct)));


    // FOR struct
    // ast* forId = ast_new_identifier("x");
    // ast* forId_2 = ast_new_identifier("x");
    // ast* forId_3 = ast_new_identifier("x");
    // ast* forId_4 = ast_new_identifier("x");
    // ast* forId_5 = ast_new_identifier("x");
    // ast* forIncrVar = ast_new_identifier("i");
    // ast* forIncrVar_2 = ast_new_identifier("i");
    // ast* forIncrVar_3 = ast_new_identifier("i");
    // ast* forValTest = ast_new_number(15);
    // ast* forIdInit = ast_new_number(0);
    // ast* forAfct1 = ast_new_binaryOperation(AST_OP_AFCT, forId, forIdInit);
    // ast* forAfctInstr = ast_new_Instruction(forAfct1);
    // ast* forCondTest = ast_new_binaryOperation(AST_BOOL_LEQ, forId_2, forValTest);
    // ast* forCondBoolExpr = ast_new_boolExpr(forCondTest, NULL, NULL);
    //
    // ast* forTrueAfct = ast_new_binaryOperation(AST_OP_AFCT, forId_3, ast_new_binaryOperation(AST_OP_ADD, forId_4, forIncrVar));
    // ast* forTrueIncr = ast_new_unaryOperation(AST_OP_INCR, forId_5);
    // ast* forInstrTrueAfct = ast_new_Instruction(forTrueAfct);
    // ast* forInstrTrueIncr = ast_new_Instruction(forTrueIncr);
    // ast* forTrueInstr = ast_concat(forInstrTrueAfct, forInstrTrueIncr);
    // ast* forInitStatement = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, forIncrVar_2), ast_new_number(0));
    // ast* forUpdateStatement = ast_new_unaryOperation(AST_OP_INCR, forIncrVar_3);
    //
    // placeGoto(forCondBoolExpr, forTrueInstr, NULL);
    // ast* forStruct = ast_new_controlStructure(AST_FOR, forCondBoolExpr, forTrueInstr, NULL, forInitStatement, forUpdateStatement);
    // ast* forInstr = ast_concat(forAfctInstr, ast_new_Instruction(forStruct));

    // Table test

      // ID
    ast* tabId = ast_new_identifier("tab");
      // Dimensions
    ast* dim1 = ast_new_tabDimension(ast_new_number(2));
    ast* dim2 = ast_new_tabDimension(ast_new_number(3));
    ast* tabDim = ast_concat(dim1, dim2);
      // Elements
    //first block
    ast* elem00 = ast_new_tabElements(ast_new_number(0));
    ast* elem01 = ast_new_tabElements(ast_new_number(1));
    ast* temp_elem0 = ast_concat(elem00, elem01);
    ast* elem02 = ast_new_tabElements(ast_new_number(2));
    ast* temp_elem1 = ast_concat(temp_elem0, elem02);
    ast* tabBlock0 = ast_new_tableElementsBlock(temp_elem1);
    // Second block
    ast* elem10 = ast_new_tabElements(ast_new_number(10));
    ast* elem11 = ast_new_tabElements(ast_new_number(11));
    ast* temp_elem3 = ast_concat(elem10, elem11);
    ast* elem12 = ast_new_tabElements(ast_new_number(12));
    ast* temp_elem4 = ast_concat(temp_elem3, elem12);
    ast* tabBlock1 = ast_new_tableElementsBlock(temp_elem4);
    ast* tabElem = ast_concat(tabBlock0, tabBlock1);
      // AST table declaration
   ast* tabDecl = ast_new_tabDeclaration(tabId, tabDim, tabElem);
   ast* tabInstr = ast_new_Instruction(tabDecl);

    // Main function
    // ast* arg1ID = ast_new_identifier("arg1");
    // ast* arg2ID = ast_new_identifier("arg2");
    // ast* arg1Decl = ast_new_binaryOperation(AST_OP_DECL, arg1ID, NULL);
    // ast* arg2Decl = ast_new_binaryOperation(AST_OP_DECL, arg2ID, NULL);
    // ast* arg1ARG = ast_new_argument(arg1Decl);
    // ast* arg2ARG = ast_new_argument(arg2Decl);
    // ast* tempArg = ast_concat(arg1ARG, arg2ARG);

    ast* funcID = ast_new_identifier("main");
    astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(ast_concat(temp, instr1), ast_concat(afctInstr, ast_concat(ifToInstr, tabInstr))));

    print_ast(astTest, 0);
    ast_free(astTest);

    return 0;
}
