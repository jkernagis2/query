testv1: testv1.c init.c logs.c d_grep.c
	gcc -g -o distrib_grep -lpthread -lnsl testv1.c init.c logs.c d_grep.c

clean:
	-rm -f distrib_grep *.o *~ *.exe


