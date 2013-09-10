//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             server.c                                 //
//  Function definitions for server functions.                          //
//                                                                      //
//  Built on                                                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "server.h"

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */


 /* function prototype */
void error(char *message)
{
    perror(message);
    exit(1);
}

int main(int argc, char *argv[])
{
     int socket_file_desc, new_socket_file_desc, port_number, clilen, pid;
     struct sockaddr_in server_address, client_address;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     socket_file_desc = socket(AF_INET, SOCK_STREAM, 0);
     if (socket_file_desc < 0) 
        error("ERROR opening socket");
     memset( (char*)(&server_address),'\0' ,(sizeof(server_address)) );
     port_number = atoi(argv[1]);
     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = INADDR_ANY;
     server_address.sin_port = htons(port_number);
     if (bind(socket_file_desc, (struct sockaddr *) &server_address,
              sizeof(server_address)) < 0) 
              error("ERROR on binding");
     listen(socket_file_desc,5);
     clilen = sizeof(client_address);
     while (1) {
         new_socket_file_desc = accept(socket_file_desc, 
               (struct sockaddr *) &client_address, &clilen);
         if (new_socket_file_desc < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(socket_file_desc);
             connect_daemon(new_socket_file_desc);
             exit(0);
         }
         else close(new_socket_file_desc);
     } /* end of while */
     return 0; /* we never get here */
}

/******** connect_daemon() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void connect_daemon(int sock)
{
   int n;
   char buffer[256];
      
   memset(buffer, '\0', 256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
