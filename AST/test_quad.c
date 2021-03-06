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
  ast* printi_name2 = ast_new_identifier("printi");
  ast* printi_argVal2 = ast_new_number(0);
  ast* printi_args2 = ast_new_argument(printi_argVal2);
  ast* printi_call2 = ast_new_functionCall(printi_name2, printi_args2);
  ast* false_instr = ast_new_Instruction(printi_call2);

  placeGoto(first_boolExpr, true_instr, false_instr);
  ast* if_ast =  ast_new_controlStructure(AST_IF, first_boolExpr, true_instr, false_instr, NULL, NULL);

  ast* if_instr = ast_new_Instruction(if_ast);

  ast* temp3 = ast_concat(printi_inst, if_instr);

  // FOR struct
  ast* forId = ast_new_identifier("x");
  ast* forId_2 = ast_new_identifier("x");
  ast* forId_3 = ast_new_identifier("x");
  ast* forId_4 = ast_new_identifier("x");
  ast* forId_5 = ast_new_identifier("x");
  ast* forIncrVar = ast_new_identifier("i");
  ast* forIncrVar_2 = ast_new_identifier("i");
  ast* forIncrVar_3 = ast_new_identifier("i");
  ast* forValTest = ast_new_number(15);
  ast* forIdInit = ast_new_number(0);
  ast* forAfct1 = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL,forId), forIdInit);
  ast* forAfctInstr = ast_new_Instruction(forAfct1);
  ast* forCondTest = ast_new_binaryOperation(AST_BOOL_LEQ, forId_2, forValTest);
  ast* forCondBoolExpr = ast_new_boolExpr(forCondTest, NULL, NULL);

  ast* forTrueAfct = ast_new_binaryOperation(AST_OP_AFCT, forId_3, ast_new_binaryOperation(AST_OP_ADD, forId_4, forIncrVar));
  ast* forTrueIncr = ast_new_unaryOperation(AST_OP_INCR, forId_5);
  ast* forInstrTrueAfct = ast_new_Instruction(forTrueAfct);
  ast* forInstrTrueIncr = ast_new_Instruction(forTrueIncr);
  ast* forTrueInstr = ast_concat(forInstrTrueAfct, forInstrTrueIncr);
  ast* forInitStatement = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, forIncrVar_2), ast_new_number(0));
  ast* forUpdateStatement = ast_new_unaryOperation(AST_OP_INCR, forIncrVar_3);

  placeGoto(forCondBoolExpr, forTrueInstr, NULL);
  ast* forStruct = ast_new_controlStructure(AST_FOR, forCondBoolExpr, forTrueInstr, NULL, forInitStatement, forUpdateStatement);
  ast* forInstr = ast_concat(forAfctInstr, ast_new_Instruction(forStruct));

  ast* temp4 = ast_concat(temp3, forInstr);

  ast* printi_nameFor = ast_new_identifier("printi");
  ast* printi_argValFor = ast_new_identifier("x");
  ast* printi_argsFor = ast_new_argument(printi_argValFor);
  ast* printi_callFor = ast_new_functionCall(printi_nameFor, printi_argsFor);
  ast* printFor_instr = ast_new_Instruction(printi_callFor);

  ast* temp5 = ast_concat(temp4, printFor_instr);

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

  ast* temp6 = ast_concat(temp5, tabInstr);

  // Table access test
    // ID
  ast* tabIdA = ast_new_identifier("tab");
    // Dimensions
  ast* dim1A = ast_new_tabDimension(ast_new_number(1));
  ast* dim2A = ast_new_tabDimension(ast_new_number(2));
  ast* tabDimA = ast_concat(dim1A, dim2A);

  ast* tabAcc = ast_new_tableAccess(tabIdA, tabDimA);
  ast* tabAccAffId = ast_new_identifier("test");
  ast* tabAccAfct = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, tabAccAffId), tabAcc);
  ast* tabAccInstr = ast_new_Instruction(tabAccAfct);

  ast* tempTableOp = ast_concat(temp6, tabAccInstr);

  // Stencils tests

    // Stencil declaration
  ast* stenId = ast_new_identifier("sten");
    // Dimensions
  ast* dim1Sten = ast_new_tabDimension(ast_new_number(3));
  ast* dim2Sten = ast_new_tabDimension(ast_new_number(3));
  ast* stenDim = ast_concat(dim1Sten, dim2Sten);
    // Elements
  //first block
  ast* elem00Sten = ast_new_tabElements(ast_new_number(0));
  ast* elem01Sten = ast_new_tabElements(ast_new_number(1));
  ast* temp_elem0Sten = ast_concat(elem00Sten, elem01Sten);
  ast* elem02Sten = ast_new_tabElements(ast_new_number(2));
  ast* temp_elem1Sten = ast_concat(temp_elem0Sten, elem02Sten);
  ast* tabBlock0Sten = ast_new_tableElementsBlock(temp_elem1Sten);
  // Second block
  ast* elem10Sten = ast_new_tabElements(ast_new_number(10));
  ast* elem11Sten = ast_new_tabElements(ast_new_number(11));
  ast* temp_elem3Sten = ast_concat(elem10Sten, elem11Sten);
  ast* elem12Sten = ast_new_tabElements(ast_new_number(12));
  ast* temp_elem4Sten = ast_concat(temp_elem3Sten, elem12Sten);
  ast* tabBlock1Sten = ast_new_tableElementsBlock(temp_elem4Sten);
  ast* StenElem = ast_concat(tabBlock0Sten, tabBlock1Sten);
  // Third block
  ast* elem20Sten = ast_new_tabElements(ast_new_number(10));
  ast* elem21Sten = ast_new_tabElements(ast_new_number(11));
  ast* temp_elem5Sten = ast_concat(elem20Sten, elem21Sten);
  ast* elem22Sten = ast_new_tabElements(ast_new_number(12));
  ast* temp_elem6Sten = ast_concat(temp_elem5Sten, elem22Sten);
  ast* tabBlock2Sten = ast_new_tableElementsBlock(temp_elem6Sten);
  StenElem = ast_concat(StenElem, tabBlock2Sten);
    // AST stencil declaration
  ast* stenDecl = ast_new_tabDeclaration(stenId, stenDim, StenElem);
  stenDecl->type = AST_STENCIL_DECL;
  ast* stenInstr = ast_new_Instruction(stenDecl);
  ast* stenDeclAST = ast_concat(tempTableOp, stenInstr);

    // Stencil operation test

  // Table access test
    // ID
  ast* tabIdStenOp = ast_new_identifier("tab");
    // Dimensions
  ast* dim1StenOp = ast_new_tabDimension(ast_new_number(1));
  ast* dim2StenOp = ast_new_tabDimension(ast_new_number(2));
  ast* tabDimStenOp = ast_concat(dim1StenOp, dim2StenOp);
  ast* rightStenOp = ast_new_tableAccess(tabIdStenOp, tabDimStenOp);
  ast* leftStenOp = ast_new_identifier("sten");
  ast* stenOpTest = ast_new_binaryOperation(AST_OP_STEN, leftStenOp, rightStenOp);
  // ast* afctStenOpTest = ast_new_binaryOperation(AST_OP_AFCT, ast_new_unaryOperation(AST_OP_DECL, ast_new_identifier("testSten")), stenOpTest);

  ast* stenOpTestInstr = ast_new_Instruction(stenOpTest);
  ast* stenOpTestAST = ast_concat(stenDeclAST, stenOpTestInstr);


  ast* funcID = ast_new_identifier("main");
  astTest = ast_new_functionDefinition(funcID, NULL, ast_concat(temp2, stenOpTestAST));

  print_ast(astTest, 0);

  symTable* symTableTest = genSymTable_init(astTest);

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
