CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
TESTAST=test_AST

testast: $(TESTAST)

test_AST: ast.o test_AST.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
