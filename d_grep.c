#include <stdlib.h>
#include <string.h>

void d_grep(char* command_buffer, int machine_num) {

    int i,j;                      // Index vars
    int command_flag;             // 0 = no flags so default grep, 1 = key grep, 2 = value grep
    int search_str_size;          // Size of search string alone
    int sys_str_size;             // Size of search string alone
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
    system_buffer = malloc((54 + search_str_size) * sizeof(char));
    // Copy over the search expression
    j = 0;
    while(command_buffer[i] != '\0'){
        gawk_buffer[j] = command_buffer[i];
        i++;
        j++;
    }

    // Generating console command string
    // 14 + search string size + 38 = Size of string output to system
    sprintf(system_buffer,"%s%s%s%d%s%d%s","gawk -F: '$0~/",gawk_buffer,"/{print$0}' machine.",machine_num,".log > result",machine_num,".tmp\n");
    printf(system_buffer);
    // Setting whether we want to search the whole line/key/values
    switch(command_flag){
        case(0):
            break;
        case(1):
            system_buffer[11] = '1';
            break;
        case(2):
            system_buffer[11] = '2';
            break;
        default:
            break;
    }

    // Make the system call to do gawk
    system(system_buffer);

    // Results of the gawk/grep are in results.tmp, ready to be sent out or whatever we need to do
    return;
}