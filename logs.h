//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.h                                  //
//  Includes and declarations for the logs.c file.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef LOGS_H
#define LOGS_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void error_f(char *msg);
void gen_logs(int machine_num);
int verify_logs(int test_num);

#endif // LOGS_H
