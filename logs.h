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

void log_event(char* event, struct single_tracker_s machine);

#endif // LOGS_H
