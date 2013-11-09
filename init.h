#ifndef INIT_H_
#define INIT_H_

//extern void init(void);

//extern void multicast(const char *message);

#include "d_grep.h"
#include "shared.h"
#include <semaphore.h>
#include <math.h>

void *grep_recv_thread_main(void *discard);
void *goss_recv_thread_main(void *discard);
void *gossip_thread_main(void *discard);
void *monitor_thread_main(void *discard);

void getIP(void);

void join(gossip_s* new_gossip);
void leave(int index, int type);


void init(int type, char * servIP);

void multicast(const char *message);

#endif