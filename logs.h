//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.h                                  //
//  Includes and declarations for the logs.c file.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef LOGS_H
#define LOGS_H

#include "shared.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void error_f(char *msg);
void gen_logs(int machine_num);
int verify_logs(int test_num);
void log_event(int my_id, int num_machines, char* event, gossip_s* machine);

#endif // LOGS_H

