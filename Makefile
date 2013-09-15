testv1: testv1.c init.c
	gcc -o testv1 -lpthread -lnsl testv1.c init.c 

clean:
	-rm -f testv1 *.o *~ *.exe


