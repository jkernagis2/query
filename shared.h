#ifndef __SHARED_H_
#define __SHARED_H_




/* Multicast message (NUL terminated) to multicast group (implicit) */
void multicast(const char *message);

typedef struct {
	char id;
	int nid;
	char command[20];
	char message[50000];
    int bytes_sent;
} mess_s;


#endif
