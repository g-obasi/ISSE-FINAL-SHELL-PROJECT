CFLAGS = -Wall -g

all: plaidsh ps_test pl_test et_test

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

et_test: et_test.o tokenize.o clist.o
	gcc $(CFLAGS) -o et_test et_test.o tokenize.o  clist.o

et_test.o: et_test.c tokenize.h parse.h
	gcc $(CFLAGS) -c et_test.c

parse.o: parse.c parse.h pipeline.c pipeline.h
	gcc $(CFLAGS) -c parse.c pipeline.h

ps_test: ps_test.o parse.o tokenize.o clist.o
	gcc $(CFLAGS) -o ps_test ps_test.o parse.o pipeline.o tokenize.o clist.o


pl_test: pl_test.o pipeline.o clist.o tokenize.o execute.o parse.o
	gcc $(CFLAGS) -o pl_test pl_test.o pipeline.o clist.o tokenize.o execute.o parse.o


clean:
	rm -f plaidsh ps_test pl_test et_test *.o
