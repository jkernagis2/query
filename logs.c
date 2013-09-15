//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.c                                  //
//  Function definitions for the generation of random log files.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "logs.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void error_f(char *msg)
{
    perror(msg);
    exit(0);
}

void gen_logs(int machine_num){
    
    char buffer[14];    // Buffer for the file name so we can dynamically name the file based on machine ID
    buffer[13] = '\0';
    sprintf(buffer,"machine.%d.log",machine_num); // log name is based on machine ID
    FILE* fp = fopen(buffer,"w");
    int r;
    srand(time(NULL)); // Setup random number generation for picking user numbers
    
    switch(machine_num){
        case 1:
            fprintf(fp, "INFO:USER_1 LOGON\n");
            fprintf(fp, "INFO:USER_91 LOGON\n");
            fprintf(fp, "INFO:USER_90 LOGON\n");
            fprintf(fp, "ERROR:USER_1 INVALID_REQUEST\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "ERROR:USER_1 CONNECTION_TIMEOUT\n");
            fprintf(fp, "INFO:USER_2 LOGON\n");
            fprintf(fp, "INFO:USER_3 LOGON\n");
            fprintf(fp, "INFO:USER_4 LOGON\n");
            fprintf(fp, "ERROR:SERVER OVERLOADED\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 INVALID_PASSWORD\n");
            fprintf(fp, "WARNING:USER_5 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "INFO:USER_6 LOGON\n");
            fprintf(fp, "WARNING:USER_6 OS OUT OF DATE\n");
            fprintf(fp, "INTRUSION:USER_6 ACCOUNT COMPROMISED\n");
            fprintf(fp, "INFO:USER_6 FORCE DISCONNECT\n");
            fprintf(fp, "QUERY:USER_3 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_92 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_12 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_32 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_45 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_56 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_34 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_453 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_76 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_65 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_657 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_764 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_3456 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_345 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_234 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_1234 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_65 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_74 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_345 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_21 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_12 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_23 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_76 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_87 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_23 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_27 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_11 QUERY RECIEVED\n");
            fprintf(fp, "QUERY:USER_18 QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER_2 LOGOFF\n");
            break;
        case 2:
            fprintf(fp, "ERROR:USER_10 CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER_9 OS OUT OF DATE\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "INFO:USER_9 FORCE DISCONNECT\n");
            fprintf(fp, "ERROR:USER_11 INVALID_REQUEST\n");
            fprintf(fp, "ERROR:USER_3 SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER_13 INVALID_PASSWORD\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "INFO:USER_13 LOGON\n");
            fprintf(fp, "QUERY:USER_13 QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER_13 LOGOFF\n");
            break;
        case 3:
            fprintf(fp, "INFO:USER_15 LOGOFF\n");
            fprintf(fp, "INFO:USER_23 LOGON\n");
            fprintf(fp, "WARNING:USER_24 INVALID_PASSWORD\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "ERROR:USER_17 INVALID_REQUEST\n");
            fprintf(fp, "QUERY:USER_17 QUERY RECIEVED\n");
            fprintf(fp, "ERROR:USER_17 CONNECTION_TIMEOUT\n");
            fprintf(fp, "ERROR:USER_19 SERVER OVERLOADED\n");
            fprintf(fp, "WARNING:USER_24 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "WARNING:USER_45 OS OUT OF DATE\n");
            fprintf(fp, "INFO:USER_45 FORCE DISCONNECT\n");
            fprintf(fp, "INTRUSION:USER_45 ACCOUNT COMPROMISED\n");
            break;
        case 4:
            fprintf(fp, "ERROR:USER_59 CONNECTION_TIMEOUT\n");
            fprintf(fp, "WARNING:USER_78 INVALID_PASSWORD\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "WARNING:USER_78 LOGON ATTEMPTS EXCEEDED\n");
            fprintf(fp, "INTRUSION:USER_89 ACCOUNT COMPROMISED\n");
            fprintf(fp, "ERROR:USER_89 SERVER OVERLOADED\n");
            fprintf(fp, "%d:%d\n", rand()%999, rand()%999);
            fprintf(fp, "INFO:USER_89 FORCE DISCONNECT\n");
            fprintf(fp, "QUERY:USER QUERY RECIEVED\n");
            fprintf(fp, "INFO:USER LOGON\n");
            fprintf(fp, "INFO:USER_23 LOGOFF\n");
            fprintf(fp, "ERROR:USER_99 INVALID_REQUEST\n");
            break;
        default:
            fclose(fp);
            error_f("Invalid machine_num");
            break;
    }
    fclose(fp);
    return;
}

int verify_logs(int test_num){

    
}