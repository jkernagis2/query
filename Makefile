testv1: testv1.c init.c logs.c d_grep.c
	gcc -o distrib_grep -lpthread -lnsl testv1.c init.c logs.c d_grep.c

clean:
	-rm -f testv1 *.o *~ *.exe


