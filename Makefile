
CC=gcc
CFLAGS=-W -Wall

all:
	yacc -d grammar.y
	flex lexical_analysis.lex
	$(CC) y.tab.c lex.yy.c -ll -ly

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h
