debug: main.c network.c logs.c d_grep.c
	gcc -g -o debug -lpthread -lnsl -lm main.c network.c logs.c d_grep.c

release: main.c network.c logs.c d_grep.c
	gcc -o release -lpthread -lnsl -lm main.c network.c logs.c d_grep.c

clean:
	-rm -f debug release ./m1/debug ./m2/debug ./m3/debug ./m4/debug *.o *~ *.exe
