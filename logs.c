//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.c                                  //
//  Function definitions for the generation of random log files.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "logs.h"

void gen_logs(){
    int num_lines = 0;
    int machine_num = 0;
    char buffer[14];
    int i,r;
    srand(time(NULL));
    
    printf("How many lines to generate? :::> ");
    scanf("%d",&num_lines);
    printf("What machine am I? :::> ");
    scanf("%d",&machine_num);    

    buffer[13] = '\0';
    sprintf(buffer,"machine.%d.log",machine_num);
    FILE* fp = fopen(buffer,"w");
    
    fprintf(fp, "[SYSTEM]:::::LOGGING_SYSTEM_ONLINE\n");
    
    for(i = 0; i < num_lines; i++){
        r = rand() % 10;
        switch(r){
            
            case 0:
                fprintf(fp, "%s%d%s", "[INFO]:::::::USER_", rand()%200, "::LOGON\n");
                break;
            case 1:
                fprintf(fp, "%s%d%s", "[INFO]:::::::USER_", rand()%200, "::LOGOFF\n");
                break;
            case 2:
                fprintf(fp, "%s%d%s", "[ERROR]::::::USER_", rand()%200, "::INVALID_REQUEST\n");
                break;
            case 3:
                fprintf(fp, "%s%d%s", "[ERROR]::::::USER_", rand()%200, "::CONNECTION_TIMEOUT\n");
                break;
            case 4:
                fprintf(fp, "%s%d%s", "[ERROR]::::::USER_", rand()%200, "::SERVER_OVERLOADED\n");
                break;
            case 5:
                fprintf(fp, "%s%d%s", "[WARNING]::::USER_", rand()%200, "::INVALID_PASSWORD\n");
                break;
            case 6:
                fprintf(fp, "%s%d%s", "[WARNING]::::USER_", rand()%200, "::LOGON_ATTEMPTS_EXCEEDED\n");
                break;
            case 7:
                fprintf(fp, "%s%d%s", "[WARNING]::::USER_", rand()%200, "::OS_OUT_OF_DATE\n");
                break;
            case 8:
                fprintf(fp, "%s%d%s", "[INTRUSION]::USER_", rand()%200, "::ACCOUNT_COMPROMISED\n");
                break;
            case 9:
                fprintf(fp, "%s%d%s", "[QUERY]::::::USER_", rand()%200, "::QUERY_RECIEVED\n");
                break;
            default:
                fprintf(fp, "%s%d%s", "[SYSTEM]::WHAT_HAPPENED\n");
                break;
        }
    }
    fprintf(fp, "[SYSTEM]:::::SYSTEM_SHUTDOWN::%d\n",time(NULL));
    fputc('\n',fp);
    fclose(fp);
    return;
}
