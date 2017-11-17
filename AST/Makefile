CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
TESTAST=test_AST
TESTSYMTBL = test_symTable

testast: $(TESTAST)

testSYMTable: $(TESTSYMTBL)

test_symTable: symbolTable.o test_symTable.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_AST: ast.o test_AST.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
