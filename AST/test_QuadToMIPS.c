#include "quadToMIPS.h"

int main()
{

  // Creation de l'AST pour les tests
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
  astTest = mult2;

  symTable* symTableTest = symTable_init();

  genSymTable_ast(astTest, symTableTest);
  codegen* cgBis = codegen_init();
  cgBis = codegen_ast(cgBis, astTest, symTableTest);

  genMIPS_genCode("test_quadToMIPS.test", cgBis->code, symTableTest);

  // Frees and close here
  symTable_free(symTableTest);
  ast_free(astTest);
  codegen_free(cgBis);
}
