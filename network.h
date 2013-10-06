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

void *receive_thread_main(void *discard);

void getIP(void);

void init_others(void);

extern void init(void);

extern void multicast(const char *message);

#endif
