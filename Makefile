all: testv1 front
testv1: testv1.c init.c logs.c d_grep.c keys.c
	gcc -g -o distrib_grep -lpthread -lnsl -lm -lssl testv1.c init.c logs.c d_grep.c keys.c
front: front.c request.c
	gcc -g -o front -lpthread -lnsl -lm -lssl front.c request.c
clean:
	-rm -f distrib_grep front ./m*/distrib_grep ./m*/machine.* *.o *~ *.exe


