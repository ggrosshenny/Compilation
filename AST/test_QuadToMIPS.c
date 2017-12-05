#include "quadToMIPS.h"

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

  ast* temp2 = ast_concat(temp, instr1);

  ast* printi_name = ast_new_identifier("printi");
  ast* printi_argVal = ast_new_number(42);
  ast* printi_args = ast_new_argument(printi_argVal);
  ast* printi_call = ast_new_functionCall(printi_name, printi_args);
  ast* printi_inst = ast_new_Instruction(printi_call);

  // If structure
  ast* left_or_boolExpr0 = ast_new_number(5);
  ast* left_or_boolExpr1 = ast_new_number(4);
  ast* left_or_boolExpr = ast_new_binaryOperation(AST_BOOL_EQ, left_or_boolExpr0, left_or_boolExpr1);
  ast* right_or_boolExpr0 = ast_new_number(5);
  ast* right_or_boolExpr1 = ast_new_number(4);
  ast* right_or_boolExpr = ast_new_binaryOperation(AST_BOOL_GT, right_or_boolExpr0, right_or_boolExpr1);
  ast* second_boolExpr = ast_new_boolExpr(right_or_boolExpr, NULL, NULL);
  ast* first_boolExpr = ast_new_boolExpr(left_or_boolExpr, NULL, second_boolExpr);
    // TRUE
  ast* printi_name1 = ast_new_identifier("printi");
  ast* printi_argVal1 = ast_new_number(1);
  ast* printi_args1 = ast_new_argument(printi_argVal1);
  ast* printi_call1 = ast_new_functionCall(printi_name1, printi_args1);
  ast* true_instr = ast_new_Instruction(printi_call1);
    // FALSE
  // ast* printi_name2 = ast_new_identifier("printi");
  // ast* printi_argVal2 = ast_new_number(0);
  // ast* printi_args2 = ast_new_argument(printi_argVal2);
  // ast* printi_call2 = ast_new_functionCall(printi_name2, printi_args2);
  // ast* false_instr = ast_new_Instruction(printi_call2);

  placeGoto(first_boolExpr, true_instr, NULL);
  ast* if_ast =  ast_new_controlStructure(AST_WHILE, first_boolExpr, true_instr, NULL);

  ast* if_instr = ast_new_Instruction(if_ast);

  ast* temp3 = ast_concat(printi_inst, if_instr);

  ast* funcID = ast_new_identifier("main");
  astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(temp2, temp3));

  symTable* symTableTest = genSymTable_init(astTest);

  genSymTable_ast(astTest, symTableTest);
  codegen* cgBis = codegen_init();
  cgBis = codegen_ast(cgBis, astTest, symTableTest);

  genMIPS_genCode("test_quadToMIPS.test", cgBis, symTableTest);

  // Frees and close here
  symTable_free(symTableTest);
  ast_free(astTest);
  codegen_free(cgBis);
}
