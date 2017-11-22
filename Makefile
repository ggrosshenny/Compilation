
CC=gcc
CFLAGS=-W -Wall
LDFLAGS=

all:
	yacc -d -Wconflicts-sr grammar.y
	flex lexical_analysis.lex
	$(CC) y.tab.c lex.yy.c AST/ast.c -ll -ly

test_quad: ast.o symbolTable.o astToST.o quad.o test_quad.o
		$(CC) -o $@ $^ $(LDFLAGS)

test_quad.o: QUAD/test_quad.c QUAD/quad.h
		$(CC) -o $@ -c $< $(CFLAGS)

quad.o: QUAD/quad.c AST/astToST.h
		$(CC) -o $@ -c $< $(CFLAGS)

astToST.o: AST/astToST.c AST/ast.h AST/symbolTable.h
		$(CC) -o $@ -c $< $(CFLAGS)

symbolTable.o: AST/symbolTable.c
		$(CC) -o $@ -c $< $(CFLAGS)

ast.o: AST/ast.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h *.o
