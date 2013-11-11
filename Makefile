all: testv1 insert lookup update delete
testv1: testv1.c init.c logs.c d_grep.c keys.c
	gcc -g -o distrib_grep -lpthread -lnsl -lm -lssl testv1.c init.c logs.c d_grep.c keys.c
insert: insert.c request.c
	gcc -g -o insert -lpthread -lnsl -lm -lssl insert.c request.c
lookup: lookup.c request.c
	gcc -g -o lookup -lpthread -lnsl -lm -lssl lookup.c request.c
update: update.c request.c
	gcc -g -o update -lpthread -lnsl -lm -lssl update.c request.c
delete: delete.c request.c
	gcc -g -o delete -lpthread -lnsl -lm -lssl delete.c request.c
clean:
	-rm -f distrib_grep insert lookup update delete ./m*/distrib_grep ./m*/machine.* *.o *~ *.exe


