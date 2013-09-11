//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             server.c                                 //
//  Function definitions for server functions. Port number should be    //
//  passed in as an argument.                                           //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "server.h"

void error(char *message)
{
    perror(message);
    exit(1);
}

int main(int num_arguments, char *arguments_buffer[])
{
    // Variable declarations: file descriptors, port #, size of
    // client's address, process ID, server/client address.
    int socket_file_desc, new_socket_file_desc, port_number, client_address_size, pid;
    struct sockaddr_in server_address, client_address;

    // Check if port was specified.
    if (num_arguments < 2) {
        error("WARNING: No port given.\n");
    }

    // Get a socket file descriptor from system.
    socket_file_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_desc < 0){
        error("WARNING: Problem getting socket.");
    }

    // Clear the server address structure .
    memset( (char*)(&server_address),'\0' ,(sizeof(server_address)) );

    // Get the port number from the arguments buffer. atoi converts string numbers to integer.
    //    Example: "23" is a string of 2 chars, converts to 23, a single int.
    port_number = atoi(arguments_buffer[1]);

    // Setting up server address structure.
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    // Bind the socket to our server (this program) address.
    if (bind(socket_file_desc, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        error("WARNING: Problem binding socket.");
    }

    // Listen for a connection on the socket.
    listen(socket_file_desc,5);

    // Just setting client address size variable as needed for accept().
    client_address_size = sizeof(client_address);

    // Main program loop, keep managing connections indefinitely.
    while (1) {

        // Set up a file descriptor for the client connection.
        new_socket_file_desc = accept(socket_file_desc, (struct sockaddr *) &client_address, &client_address_size);

        // Error handling.
        if (new_socket_file_desc < 0){
            error("WARNING: Problem accepting socket connection.");
        }

        // Fork so that we can manage multiple connections at the same time.
        pid = fork();

        // Error handling.
        if (pid < 0){
            error("WARNING: Problem forking process.");
        }

        // Otherwise, close the old file descriptor, and run the connection handler.
        // The handler will handle creating and maintaining the client address book.
        if (pid == 0){
            close(socket_file_desc);
            connect_daemon(new_socket_file_desc, port_number);
            exit(0);
        }else{
            close(new_socket_file_desc);
        }
     } // End of program loop.
     return 0; // We never get to this return due to the while loop.
}

////////////////////////////////////////////////
//             connect_daemon()               //
//  Handles all sending/receiving of data     //
////////////////////////////////////////////////
void connect_daemon(int socket)
{
   int n;
   char buffer[256];
   memset(buffer, '\0', 256);
   n = read(socket,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(socket,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
