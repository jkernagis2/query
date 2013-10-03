//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            network.h                                 //
//  Function definitions for initializing and maintaining the network   //
//   interface.                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef INIT_H_
#define INIT_H_

#include "d_grep.h"
#include "shared.h"

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

void *receive_thread_main(void *discard);

void getIP(void);

void init_others(void);

extern void init(void);

extern void multicast(const char *message);

#endif
