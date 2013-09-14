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
    
    // Buffer for the file name so we can dynamically name the file based on machine ID
    char buffer[14];
    buffer[13] = '\0';
    sprintf(buffer,"machine.%d.log",machine_num);
    FILE* fp = fopen(buffer,"w");

    switch(machine_num){
        case 1:
            fprintf(fp, "INFO:USER_1 LOGON\n");
            fprintf(fp, "ERROR:USER_1 INVALID_REQUEST\n");
            fprintf(fp, "ERROR:USER_1 CONNECTION_TIMEOUT\n");
            fprintf(fp, "INFO:USER_2 LOGON\n");
            fprintf(fp, "INFO:USER_3 LOGON\n");
            fprintf(fp, "INFO:USER_4 LOGON\n");
            fprintf(fp, "ERROR:SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "INFO:USER_6 LOGON\n");
            fprintf(fp, "WARNING:USER_6 OS OUT OF DATE\n");
            fprintf(fp, "INTRUSION:USER_6 ACCOUNT COMPROMISED\n");
            fprintf(fp, "INFO:USER_6 FORCE DISCONNECT\n");
            fprintf(fp, "QUERY:USER_3 QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER_2 LOGOFF\n");
            break;
        case 2:
            fprintf(fp, "ERROR:USER_10 CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER_9 OS OUT OF DATE\n");
            fprintf(fp, "INFO:USER_9 FORCE DISCONNECT\n");
            fprintf(fp, "ERROR:USER_11 INVALID_REQUEST\n");
            fprintf(fp, "ERROR:USER_3 SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER_13 INVALID_PASSWORD\n");
            fprintf(fp, "INFO:USER_13 LOGON\n");
            fprintf(fp, "QUERY:USER_13 QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER_13 LOGOFF\n");
            break;
        case 3:
            fprintf(fp, "INFO:USER_15 LOGOFF\n");
            fprintf(fp, "INFO:USER_23 LOGON\n");
            fprintf(fp, "WARNING:USER_24 INVALID_PASSWORD\n");
            fprintf(fp, "ERROR:USER_17 INVALID_REQUEST\n");
            fprintf(fp, "QUERY:USER_17 QUERY RECIEVED\n");
            fprintf(fp, "ERROR:USER_17 CONNECTION_TIMEOUT\n");
            fprintf(fp, "ERROR:USER_19 SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER_24 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "WARNING:USER_45 OS OUT OF DATE\n");
            fprintf(fp, "INFO:USER_45 FORCE DISCONNECT\n");
            fprintf(fp, "INTRUSION:USER_45 ACCOUNT COMPROMISED\n");
            break;
        case 4:
            fprintf(fp, "ERROR:USER_59 CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER_78 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_78 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "INTRUSION:USER_89 ACCOUNT COMPROMISED\n");
            fprintf(fp, "ERROR:USER_89 SERVER OVERLOADED\n");
            fprintf(fp, "INFO:USER_89 FORCE DISCONNECT\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "INFO:USER_23 LOGOFF\n");
            fprintf(fp, "ERROR:USER_99 INVALID_REQUEST\n");
        default:
            fclose(fp);
            error("Invalid machine_num");
            break;
    }
    fclose(fp);
    return;
}
