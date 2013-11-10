testv1: testv1.c init.c logs.c d_grep.c keys.c
	gcc -g -o distrib_grep -lpthread -lnsl -lm -lssl testv1.c init.c logs.c d_grep.c keys.c

clean:
	-rm -f distrib_grep ./m*/distrib_grep ./m*/machine.* *.o *~ *.exe


