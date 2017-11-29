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

  ast* funcID = ast_new_identifier("main");
  astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(instr0, instr1));

  symTable* symTableTest = symTable_init(astTest);

  genSymTable_ast(astTest, symTableTest);

  symTable_print(symTableTest);
  printf("\nAST :\n");
  print_ast(astTest, 0);

  symTable_free(symTableTest);
  ast_free(astTest);
}
