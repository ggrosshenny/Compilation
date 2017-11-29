#include "quad.h"


int main()
{
  // Creation de l'AST pour les tests

  // Test declaration
  ast* testId = ast_new_identifier("Test");
  ast* testVal = ast_new_number(25);
  ast* testIdDeclaration = ast_new_binaryOperation(AST_OP_DECL, testId, NULL);
  ast* testIdAffectation = ast_new_binaryOperation(AST_OP_AFCT, testIdDeclaration, testVal);

  ast* instr0 = ast_new_Instruction(testIdAffectation);

  ast* testId2 = ast_new_identifier("Test2");
  ast* testVal2 = ast_new_string("LOLOLOLOLOLOL");
  ast* testDecl2 = ast_new_binaryOperation(AST_OP_DECL, testId2, testVal2);
  ast* instr2 = ast_new_Instruction(testDecl2);

  // -2 * (10 * (test + 42))
  ast* astTest = NULL;
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
  ast* temp = ast_concat(instr0, instr2);

  ast* funcID = ast_new_identifier("main");
  astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(temp, instr1));

  symTable* symTableTest = symTable_init(astTest);
  genSymTable_ast(astTest, symTableTest);

  print_ast(astTest, 0);
  codegen* cgBis = codegen_init();
  cgBis = codegen_ast(cgBis, astTest, symTableTest);
  quadList_print(cgBis->code);
  // Memory liberation

  ast_free(astTest);
  printf("free ast\n");
  symTable_free(symTableTest);
  printf("free symTable\n");
  codegen_free(cgBis);
  printf("free codegen\n");

  return 0;
}
