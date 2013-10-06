debug: main.c network.c logs.c d_grep.c
	gcc -g -o debug -lpthread -lnsl main.c network.c logs.c d_grep.c

release: main.c network.c logs.c d_grep.c
	gcc -o release -lpthread -lnsl main.c network.c logs.c d_grep.c

clean:
	-rm -f debug release ./machine1/distrib_grep ./machine2/distrib_grep *.o *~ *.exe


