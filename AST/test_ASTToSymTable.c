#include "astToST.h"
#include <stdio.h>
#include <assert.h>

int main()
{
  // Creation de l'AST pour les tests
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

  // -2 * (10 * (test + 42))
  ast* astTest = NULL;
    // Test declaration
  //ast* testDef = ast_new_binaryOperation()
    // leftOp
  ast* leftOp = ast_new_identifier("Test");
    // rightOp
  ast* rightOp = ast_new_number(42);
    // add
  ast* add =  ast_new_binaryOperation(AST_OP_ADD, leftOp, rightOp);
    // scal
  ast* scal = ast_new_number(10);
    // scal2
  ast* scal2 = ast_new_number(2);
    // minus
  ast* minus = ast_new_unaryOperation(AST_OP_MINUS, scal2);
    // mult
  ast* mult = ast_new_binaryOperation(AST_OP_MULT, scal, add);
    // mult2
  ast* mult2 = ast_new_binaryOperation(AST_OP_MULT, minus, mult);

  ast* instr1 = ast_new_Instruction(mult2);

  ast* boolArg1 = ast_new_number(10);
  ast* boolArg2 = ast_new_number(10);
  ast* boolEq = ast_new_binaryOperation(AST_BOOL_EQ, boolArg1, boolArg2);
  ast* true_ID = ast_new_identifier("Test");
  ast* true_action = ast_new_unaryOperation(AST_OP_INCR, true_ID);
  ast* false_ID = ast_new_identifier("Test");
  ast* false_action = ast_new_unaryOperation(AST_OP_DECR, false_ID);
  ast* bool_tree = ast_new_boolExpr(boolEq, true_action, false_action);
  ast* instr2 = ast_new_Instruction(bool_tree);
  ast* tempBool = ast_concat(instr2, instr1);

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
  ast* tempTable = ast_concat(tempBool, tabInstr);

  // Table access test
    // ID
  ast* tabIdA = ast_new_identifier("tab");
    // Dimensions
  ast* dim1A = ast_new_tabDimension(ast_new_number(1));
  ast* dim2A = ast_new_tabDimension(ast_new_number(2));
  ast* tabDimA = ast_concat(dim1A, dim2A);

  ast* tabAcc = ast_new_tableAccess(tabIdA, tabDimA);
  ast* tabAccInstr = ast_new_Instruction(tabAcc);

  ast* tempTableOp = ast_concat(tempTable, tabAccInstr);

  ast* funcID = ast_new_identifier("main");
  astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(instr0, tempTableOp));

  //symTable* symTableTest = symTable_init(astTest);
  symTable* symTableTest = genSymTable_init(astTest);
  //genSymTable_ast(astTest, symTableTest);

  symTable_print(symTableTest);
  printf("\nAST :\n");
  print_ast(astTest, 0);

  symTable_free(symTableTest);
  ast_free(astTest);
}
