#ifndef INIT_H_
#define INIT_H_

// Our files
#include "shared.h"
#include "d_grep.h"
#include "keys.h"

// Libraries
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <ifaddrs.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*MP3*/
ring_n* myring;
keyval* mykv;

void init(int type, char * servIP);
void getIP();

void multicast(const char *message);

void join(gossip_s* new_gossip);
void leave(int index, int type);

void set_leave();
void rejoin();

void add_to_ring(int newid, struct in_addr new_addr);
void remove_from_ring(int id, int type);


void *grep_recv_thread_main(void *discard);
void *goss_recv_thread_main(void *discard);
void *gossip_thread_main(void *discard);
void *monitor_thread_main(void *discard);

void dequeue_sockaddr_in_n(struct sockaddr_in* input, mess_s* message);
void enqueue_sockaddr_in_n(struct sockaddr_in input, mess_s message);
void handle_req();
#endif
