CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
TESTAST = test_AST
TESTSYMBL = test_symTable
TESTASTTOSYMBL = test_astToSymTable
TESTQUADTOMIPS = test_quadToMIPS

tests: test_astToSymTable test_symTable test_AST test_quadToMIPS

test_quadToMIPS: ast.o symbolTable.o astToST.o quadToMIPS.o test_QuadToMIPS.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_astToSymTable: ast.o symbolTable.o astToST.o test_ASTToSymTable.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_symTable: symbolTable.o test_symTable.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_AST: ast.o test_AST.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(TESTAST)
	rm -rf $(TESTSYMBL)
	rm -rf $(TESTASTTOSYMBL)
	rm -rf $(TESTQUADTOMIPS)
	rm -rf test_quadToMIPS.test
