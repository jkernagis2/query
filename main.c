#include "shared.h"
#include "network.h"
#include "logs.h"
#include "d_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>



int main(int argc, char **argv){

    init(); // Calling out intialization function.
            // This sets up the needed structures to store address information

    system("rm ./backup/*");        // Remove any old log files.
    system("cp ./log/* ./backup/"); // Remove any old log files.
    system("rm ./log/*");           // Remove any old log files.

    // Main program loop
    while(1){
        char str[256];  // Input buffer
        int len;        // Used to store strlen of the input buffer

        // Get input from stdin to our input buffer
        if(fgets(str, sizeof(str), stdin) == NULL){
            if(feof(stdin)){
              break;
            }else{
                if(errno != EINTR){
                    perror("fgets");
                    break;
                }else{
                    continue;
                }
            }
        }
        // Get length of input
        len = strlen(str);

        // Trim the newline char from the buffer
        if(str[len-1] == '\n'){str[len-1] = '\0';}

        // Iff the command is to quit, break out of the program loop
        if (strncmp(str, "/quit", 5) == 0) {break;}

        // Otherwise, multicast the commmand to the other machines
        multicast(str);
    }

    return 0;
}
