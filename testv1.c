#include "shared.h"
#include "init.h"
#include "logs.h"
#include "d_grep.h"
#include "keys.h"
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
    int i;
    char key[8];
    
    while (1) {
    memset(key,'\0',8*sizeof(char));
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
            str[len-1] = '\0';
        }
        if (strncmp(str, "/quit", 5) == 0) {
            break;
        }else if(strncmp(str, "/leave",6) == 0){
        	set_leave();
        }else if(strncmp(str, "/rejoin",7) == 0){
        	rejoin();
        }else if(strncmp(str, "/insert",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            strncpy(key,&str[8], i-8);
        	insert(atoi(key), &str[i+1]);
        }else if(strncmp(str, "/lookup",7) == 0){
        	lookup(atoi(&str[8]));
        }else if(strncmp(str, "/update",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            strncpy(key,&str[8], i-8);
        	update(atoi(key), &str[i+1]);
        }else if(strncmp(str, "/delete",7) == 0){
        	delete_k(atoi(&str[8]));
        }else if(strncmp(str, "/show",5) == 0){
            local_show();
        }

        //multicast(str);
    }

    return 0;
}
