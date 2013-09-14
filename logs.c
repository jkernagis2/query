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

void gen_logs(){
    
    // Buffer for the file name so we can dynamically name the file based on machine ID
    char buffer[14];
    buffer[13] = '\0';
    sprintf(buffer,"machine.%d.log",1);
    FILE* fp1 = fopen(buffer,"w");
    sprintf(buffer,"machine.%d.log",2);
    FILE* fp2 = fopen(buffer,"w");
    sprintf(buffer,"machine.%d.log",3);
    FILE* fp3 = fopen(buffer,"w");
    sprintf(buffer,"machine.%d.log",4);
    FILE* fp4 = fopen(buffer,"w");

            fprintf(fp1, "INFO:USER_1 LOGON\n");
            fprintf(fp1, "ERROR:USER_1 INVALID_REQUEST\n");
            fprintf(fp1, "ERROR:USER_1 CONNECTION_TIMEOUT\n");
            fprintf(fp1, "INFO:USER_2 LOGON\n");
            fprintf(fp1, "INFO:USER_3 LOGON\n");
            fprintf(fp1, "INFO:USER_4 LOGON\n");
            fprintf(fp1, "ERROR:SERVER OVERLOADED\n");
            fprintf(fp1, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp1, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp1, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp1, "WARNING:USER_5 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp1, "INFO:USER_6 LOGON\n");
            fprintf(fp1, "WARNING:USER_6 OS OUT OF DATE\n");
            fprintf(fp1, "INTRUSION:USER_6 ACCOUNT COMPROMISED\n");
            fprintf(fp1, "INFO:USER_6 FORCE DISCONNECT\n");
            fprintf(fp1, "QUERY:USER_3 QUERY RECIEVED\n");
            fprintf(fp1, "INFO:USER_2 LOGOFF\n");

            fprintf(fp2, "ERROR:USER_10 CONNECTION_TIMEOUT\n");
            fprintf(fp2, "WARNING:USER_9 OS OUT OF DATE\n");
            fprintf(fp2, "INFO:USER_9 FORCE DISCONNECT\n");
            fprintf(fp2, "ERROR:USER_11 INVALID_REQUEST\n");
            fprintf(fp2, "ERROR:USER_3 SERVER OVERLOADED\n");
            fprintf(fp2, "WARNING:USER_13 INVALID_PASSWORD\n");
            fprintf(fp2, "INFO:USER_13 LOGON\n");
            fprintf(fp2, "QUERY:USER_13 QUERY RECIEVED\n");
            fprintf(fp2, "INFO:USER_13 LOGOFF\n");

            fprintf(fp3, "INFO:USER_15 LOGOFF\n");
            fprintf(fp3, "INFO:USER_23 LOGON\n");
            fprintf(fp3, "WARNING:USER_24 INVALID_PASSWORD\n");
            fprintf(fp3, "ERROR:USER_17 INVALID_REQUEST\n");
            fprintf(fp3, "QUERY:USER_17 QUERY RECIEVED\n");
            fprintf(fp3, "ERROR:USER_17 CONNECTION_TIMEOUT\n");
            fprintf(fp3, "ERROR:USER_19 SERVER OVERLOADED\n");
            fprintf(fp3, "WARNING:USER_24 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp3, "WARNING:USER_45 OS OUT OF DATE\n");
            fprintf(fp3, "INFO:USER_45 FORCE DISCONNECT\n");
            fprintf(fp3, "INTRUSION:USER_45 ACCOUNT COMPROMISED\n");

            fprintf(fp4, "ERROR:USER_59 CONNECTION_TIMEOUT\n");
            fprintf(fp4, "WARNING:USER_78 INVALID_PASSWORD\n");
            fprintf(fp4, "WARNING:USER_78 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp4, "INTRUSION:USER_89 ACCOUNT COMPROMISED\n");
            fprintf(fp4, "ERROR:USER_89 SERVER OVERLOADED\n");
            fprintf(fp4, "INFO:USER_89 FORCE DISCONNECT\n");
            fprintf(fp4, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp4, "INFO:USER LOGON\n");
            fprintf(fp4, "INFO:USER_23 LOGOFF\n");
            fprintf(fp4, "ERROR:USER_99 INVALID_REQUEST\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    return;
}
