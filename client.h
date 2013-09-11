//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             client.h                                 //
//  Includes and declarations for the client.c file.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


void error(char *msg);

int server_run(int argc, char *argv[]);


#endif // CLIENT_H
