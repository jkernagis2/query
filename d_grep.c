#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/************************************************************************/
/*                                                                      */
/*  Name:     d_grep                                                    */
/*  Inputs:   Buffer with a grep command.                               */
/*            Machine ID                                                */
/*  Output:   None                                                      */
/*  Function: Calls the system's gawk function to search machine.i.log. */
/*                                                                      */
/************************************************************************/

void d_grep(char* command_buffer, int machine_num) {

    int i;                        // Index variable
    int command_flag;             // 0 = Default grep, 1 = key grep, 2 = value grep
    int search_str_size;          // Size of search string alone
    char gawk_buffer[256];        // Internal command buffers
    char system_buffer[256];

    memset(gawk_buffer,'\0',256);   // Clear Buffers
    memset(system_buffer,'\0',256);
    
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
        i = 8; // Index of start of the expression to be grep-ed
    }else{     // If there is no flag, then we just search the whole line
        command_flag = 0;
        search_str_size = strlen(command_buffer) - 5; // Size of search expression without "grep "
        i = 5; // Index of start of the expression to be grep-ed
    }

    // Copy over the search expression
    strcpy(gawk_buffer, (command_buffer + i) );

    // Generating console command string
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
            // This will never happen.
            break;
    }

    // Make the system call to do gawk with the command string we just built
    system(system_buffer);

    // Results of the gawk/grep are in resulti.tmp, ready to be sent out or whatever we need to do
    return;
}


/************************************************************************/
/*                                                                      */
/*  Name:     combine                                                   */
/*  Inputs:   None.                                                     */
/*  Output:   None.                                                     */
/*  Function: Combines the result1.tmp through results4.tmp if present  */
/*              into a single file. Also deletes the temp files when    */
/*              done with them.                                         */
/*                                                                      */
/************************************************************************/
void combine(){

    // Open all file pointers
    FILE* fp1 = fopen("result1.tmp","r");
    FILE* fp2 = fopen("result2.tmp","r");
    FILE* fp3 = fopen("result3.tmp","r");
    FILE* fp4 = fopen("result4.tmp","r");
    FILE* fp5 = fopen("grep.output","w");
    
    // Used to store each char that we read from the file
    // When it equals EOF, we've hit the end of the file
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
    system("rm result1.tmp result2.tmp result3.tmp result4.tmp");
}



