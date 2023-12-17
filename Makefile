CFLAGS = -Wall -g

all: plaidsh ps_test pl_test tok_test

plaidsh: plaidsh.o tokenize.o clist.o parse.o pipeline.o execute.o
	gcc $(CFLAGS) -o plaidsh plaidsh.o tokenize.o clist.o parse.o pipeline.o execute.o -lc -lreadline

plaidsh.o: plaidsh.c tokenize.h clist.h parse.h pipeline.h execute.h
	gcc $(CFLAGS) -c plaidsh.c

clist.o: clist.c clist.h
	gcc $(CFLAGS) -c clist.c

tokenize.o: tokenize.c tokenize.h
	gcc $(CFLAGS) -c tokenize.c

pipeline.o: pipeline.c pipeline.h
	gcc $(CFLAGS) -c pipeline.c

execute.o: execute.c execute.h
	gcc $(CFLAGS) -c execute.c

tok_test: tok_test.o tokenize.o clist.o
	gcc $(CFLAGS) -o tok_test tok_test.o tokenize.o  clist.o

tok_test.o: tok_test.c tokenize.h parse.h
	gcc $(CFLAGS) -c tok_test.c

parse.o: parse.c parse.h pipeline.c pipeline.h
	gcc $(CFLAGS) -c parse.c pipeline.h

ps_test: ps_test.o parse.o tokenize.o clist.o execute.o
	gcc $(CFLAGS) -o ps_test ps_test.o parse.o pipeline.o tokenize.o clist.o execute.o


pl_test: pl_test.o pipeline.o clist.o tokenize.o execute.o parse.o
	gcc $(CFLAGS) -o pl_test pl_test.o pipeline.o clist.o tokenize.o execute.o parse.o


clean:
	rm -f plaidsh ps_test pl_test tok_test *.o
