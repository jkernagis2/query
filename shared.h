#ifndef __SHARED_H_
#define __SHARED_H_




/* Multicast message (NUL terminated) to multicast group (implicit) */
void multicast(const char *message);

typedef struct {
	char id;
	int nid;
	char command[20];
	char message[1024];
} mess_s;

/*** internals ***/

/* File where the list of current group members is stored */
#define GROUP_FILE  "GROUPLIST"

#endif
