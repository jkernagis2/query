//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              shared.h                                //
//  Includes and declarations for the logs.c file.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __SHARED_H_
#define __SHARED_H_

/* Basic message struct used for socket communications. */
typedef struct {
	char id;
	int nid;
	char command[20];
	char message[50000];
    int bytes_sent;
} mess_s;

typedef struct {
    int machine_id;
    int heart_counter;
    int local_clock;
} single_tracker_s;

typedef struct{
    char id;
    int nid;
    char command[20];
    struct single_tracker_s counter;
} gossip_s;

#endif
