#include "shared.h"
#include "init.h"
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



int main(int argc, char **argv) {

    //init();
    init(atoi(argv[1]),argv[2]); 
    while (1) {
        char str[256];
        int len;

        if (fgets(str, sizeof(str), stdin) == NULL) {
            if (feof(stdin)) {
              break;
            } else {
                if (errno != EINTR) {
                    perror("fgets");
                    break;
                } else {
                    continue;
                }
            }
        }
        len = strlen(str);
        /* trim newline */
        if (str[len-1] == '\n') {
            str[len-1] = 0;
        }
        if (strncmp(str, "/quit", 5) == 0) {
            break;
        }else if(strncmp(str, "/leave",6) == 0){
        	set_leave();
        }else if(strncmp(str, "/rejoin",7) == 0){
        	rejoin();
        }

        multicast(str);
    }

    return 0;
}
