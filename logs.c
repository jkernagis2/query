//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.c                                  //
//  Function definitions for the generation of random log files.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "logs.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void gen_logs(int machine_num){

    char buffer[14];



    buffer[13] = '\0';
    sprintf(buffer,"machine.%d.log",machine_num);
    FILE* fp = fopen(buffer,"w");
    fprintf(fp, "SYSTEM:LOGGING SYSTEM ONLINE\n");
    switch(machine_num){
        case 1:
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "INFO:USER LOGOFF\n");
            fprintf(fp, "ERROR:USER INVALID_REQUEST\n");
            fprintf(fp, "ERROR:USER CONNECTION_TIMEOUT\n");
            fprintf(fp, "ERROR:USER SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "WARNING:USER OS OUT OF DATE\n");
            fprintf(fp, "INTRUSION:USER ACCOUNT COMPROMISED\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            break;
        case 2:
            fprintf(fp, "SYSTEM:LOGGING SYSTEM ONLINE\n");
            fprintf(fp, "ERROR:USER CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER OS OUT OF DATE\n");
            fprintf(fp, "ERROR:USER INVALID_REQUEST\n");
            fprintf(fp, "ERROR:USER SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER INVALID_PASSWORD\n");
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "WARNING:USER LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER LOGOFF\n");
            fprintf(fp, "INTRUSION:USER ACCOUNT COMPROMISED\n");
            break;
        case 3:
            fprintf(fp, "INFO:USER LOGOFF\n");
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "WARNING:USER INVALID_PASSWORD\n");
            fprintf(fp, "ERROR:USER INVALID_REQUEST\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp, "ERROR:USER CONNECTION_TIMEOUT\n");
            fprintf(fp, "ERROR:USER SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "WARNING:USER OS OUT OF DATE\n");
            fprintf(fp, "INTRUSION:USER ACCOUNT COMPROMISED\n");
            break;
        case 4:
            fprintf(fp, "ERROR:USER CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "INTRUSION:USER ACCOUNT COMPROMISED\n");
            fprintf(fp, "ERROR:USER SERVER OVERLOADED\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "INFO:USER LOGOFF\n");
            fprintf(fp, "ERROR:USER INVALID_REQUEST\n");
            fprintf(fp, "WARNING:USER OS OUT OF DATE\n");
            break;
        default:
            fclose(fp);
            error("Please enter a valid machine ID");
            break;
    }
    fprintf(fp, "SYSTEM:SYSTEM SHUTDOWN\n");
    fputc('\n',fp);
    fclose(fp);
    return;
}
