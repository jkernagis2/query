#ifndef __SHARED_H_
#define __SHARED_H_

#include <pthread.h>

/*** unicast interface ***/

/* Should be called by multicast_init at start of program */
void unicast_init(void);

/* Multicast message (NUL terminated) to multicast group (implicit) */
void multicast(const char *message);

void get_errors();
void get_index();

/*** group maintenance ***/

extern int *mcast_members;
extern int mcast_num_members;
extern int my_id;
extern int connections[8];
extern double *val_array;

extern pthread_mutex_t member_lock;

extern int num_nodes;
extern int iterations;
extern int topology;
extern int error;
extern int num_error;
extern int f;
extern int * errors;

typedef struct {
	char id;
	char command[20];
	char message[1024];
} mess_s;

/*** internals ***/

/* File where the list of current group members is stored */
#define GROUP_FILE  "GROUPLIST"

#endif
