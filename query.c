//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              query.c                                 //
//                                                                      //
//  Function definitions for the main program.  User chooses whether    //
//  to invoke the client or server interface                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//#include "client.h"
//#include "server.h"
#include "logs.c"
#include <stdlib.h>
#include <string.h>

//#define SERVER_HARD_ADDRESS "127.0.0.1"
#define GREP_TEST_TARGET

void d_grep(char* command_buffer);

int main(){
    // Currently calls the log generator, then grep's it for

    int machine_num;

    printf("What machine am I? :::> ");
    scanf("%d",&machine_num);
    gen_logs(machine_num);

    // Now we do grep/gawk on the file
    d_grep("grep -k WARNING");

    return 0;

}

void d_grep(char* command_buffer) {

    int i,j;                      // Index vars
    int command_flag;             // 0 = no flags so default grep, 1 = key grep, 2 = value grep
    int search_str_size;                 // Size of search string alone
    int sys_str_size;                 // Size of search string alone
    char* gawk_buffer;            // Buffer
    char* system_buffer;          // Buffer

    // Check for flags, hyphen would be after "grep "
    if(command_buffer[5] == '-'){
        switch(command_buffer[6]){
            case 'k':
                command_flag = 1;
                break;
            case 'v':
                command_flag = 2;
                break;
            default:
                perror("Bad flag, defaulting to standard grep.");
                break;
        }
        search_str_size = strlen(command_buffer) - 8; // Size of search expression without "grep -k " or "grep -v "
        i = 8; // Start of the expression to be grep-ed
    }else{
        command_flag = 0;
        search_str_size = strlen(command_buffer) - 5; // Size of search expression without "grep -k " or "grep -v "
        i = 5;
    }
    gawk_buffer = malloc(search_str_size * sizeof(char));
    system_buffer = malloc((52 + search_str_size) * sizeof(char));
    // Copy over the search expression
    j = 0;
    while(command_buffer[i] != '\0'){
        gawk_buffer[j] = command_buffer[i];
        i++;
        j++;
    }

    // Generating console command string
    // 14 + search string size + 38 = Size of string output to system
    sprintf(system_buffer,"%s%s%s","gawk -F: '$0\~/",gawk_buffer,"/{print$0}' machine.1.log > result.tmp");
    printf(system_buffer);
    // Setting whether we want to search the whole line/key/values
    system_buffer[12] = command_flag;

    // Make the system call to do gawk
    system(system_buffer);

    // Results of the gawk are in results.tmp, ready to be sent out
    return;
}
