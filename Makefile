
CC=gcc
CFLAGS=-W -Wall

all:
	yacc -d -Wconflicts-sr grammar.y 
	flex lexical_analysis.lex
	$(CC) y.tab.c lex.yy.c AST/ast.c -ll -ly

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h
