//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             server.h                                 //
//                                                                      //
//  #includes and declarations for the server.c file.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *message);

void connect_daemon(int);

int main(int argc, char *argv[]);


#endif // SERVER_H
