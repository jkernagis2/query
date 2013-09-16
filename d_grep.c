#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    while(i != strlen(command_buffer)-1){
        gawk_buffer[j] = command_buffer[i];
        i++;
        j++;
    }

    // Generating console command string
    // 14 + search string size + 38 = Size of string output to system
    sprintf(system_buffer,"%s%s%s%d%s%d%s","gawk -F: '$0~/",gawk_buffer,"/{print$0}' machine.",machine_num,".log > result",machine_num,".tmp\n");

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
    printf(system_buffer);

    // Make the system call to do gawk
    system(system_buffer);
    free(gawk_buffer);
    free(system_buffer);
    
    // Results of the gawk/grep are in results.tmp, ready to be sent out or whatever we need to do
    return;
}

// Combines the result1.tmp through results4.tmp if present into a single file, grep.output.
//  Also deletes the temp files when done with them.
void combine(){
    FILE* fp1 = fopen("result1.tmp","r");
    FILE* fp2 = fopen("result2.tmp","r");
    FILE* fp3 = fopen("result3.tmp","r");
    FILE* fp4 = fopen("result4.tmp","r");
    FILE* fp5 = fopen("grep.output","w");

    int check = 0;

    if(fp1 != NULL){
        fprintf(fp5, "machine.1.log results:\n");
        while((check = fgetc(fp1)) != EOF){
            fputc(check,fp5);
        }
        fclose(fp1);
    }else{
        fprintf(fp5, "machine.1.log failure\n");
    }

    if(fp2 != NULL){
        fprintf(fp5, "machine.2.log results:\n");
        while((check = fgetc(fp2)) != EOF){
            fputc(check,fp5);
        }
        fclose(fp2);
    }else{
        fprintf(fp5, "machine.2.log failure\n");
    }

    if(fp3 != NULL){
        fprintf(fp5, "machine.3.log results:\n");
        while((check = fgetc(fp3)) != EOF){
            fputc(check,fp5);
        }
        fclose(fp3);
    }else{
        fprintf(fp5, "machine.3.log failure\n");
    }

    if(fp4 != NULL){
        fprintf(fp5, "machine.4.log results:\n");
        while((check = fgetc(fp4)) != EOF){
            fputc(check,fp5);
        }
        fclose(fp4);
    }else{
        fprintf(fp5, "machine.4.log failure\n");
    }

    fclose(fp5);
    //system("rm result1.tmp result2.tmp result3.tmp result4.tmp");
}



