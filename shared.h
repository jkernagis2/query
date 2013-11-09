#ifndef __SHARED_H_
#define __SHARED_H_




/* Multicast message (NUL terminated) to multicast group (implicit) */
/*void multicast(const char *message);

typedef struct {
	char id;
	int nid;
	char command[20];
	char message[50000];
    int bytes_sent;
} mess_s;*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <getopt.h>
#include <arpa/inet.h>

/* Basic message struct used for socket communications. */
typedef struct {
    char id;
    int nid;
    char command[20];
    char message[50000];
    int bytes_sent;
} mess_s;

/*Struct to accomadate the gossip protocol*/
typedef struct{
    struct in_addr addr; //address
    int32_t counter; //heartbeat counter
    int32_t time; //How should time be represented?/Do we even need time?
    uint8_t id[50]; //
    int32_t p_crashed; //
    int32_t has_left; //
    /*MP3*/
    int32_t ring_id;
    
} gossip_s;

typedef struct {
    int32_t num_gossip;
    gossip_s gossips[10];
}gossip_m_s;

typedef struct {
    struct in_addr addr; //address
    int value;
    struct ring* next;
    struct ring* prev;
}ring;

typedef struct {
    int key; //address
    char* value;
    struct keyval* next;
    struct keyval* prev;
}keyval;


#endif
