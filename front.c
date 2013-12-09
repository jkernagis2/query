#include <stdlib.h>
#include <string.h>
#include "request.h"

int main(int argc, char **argv) {
    int i,j;
    char key[8];
    while(1){
        memset(key,'\0',8*sizeof(char));
        char str[256];
        int len;
        if (fgets(str, sizeof(str), stdin) == NULL) {
            if(feof(stdin)){
              break;
            }
            else{
                if(errno != EINTR){
                    perror("fgets");
                    break;
                }
                else{
                    continue;
                }
            }
        }
        len = strlen(str);
        /* trim newline */
        if(str[len-1] == '\n'){str[len-1] = '\0';}
        if (strncmp(str, "/quit", 5) == 0) {
            break;
        }else if(strncmp(str, "/insert",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            for(j = i + 1; str[j] != ' ';j++){}
            strncpy(key,&str[8], i-8);
            send_req("s_insert",atoi(key),&str[j+1],atoi(&str[j]));
        }else if(strncmp(str, "/lookup",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            send_req("s_lookup",atoi(&str[8]),NULL,atoi(&str[i]));
        }else if(strncmp(str, "/update",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            for(j = i + 1; str[j] != ' ';j++){}
            strncpy(key,&str[8], i-8);
            send_req("s_update",atoi(key),&str[j+1],atoi(&str[j]));
        }else if(strncmp(str, "/delete",7) == 0){
            for(i = 8; str[i] != ' '; i++){}
            send_req("s_delete",atoi(&str[8]),NULL,atoi(&str[i]));
        }
    }
    return 0;
}
