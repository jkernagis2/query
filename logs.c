//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.c                                  //
//  Function definitions for the generation of random log files.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////



int main(){
    int num_lines, machine_num;
    char buffer[14];
    int r;
    srand(time(NULL));
    
    printf("How many lines to generate? :::> ");
    scanf("%d",&num_lines);
    printf("What machine am I? :::> ");
    scanf("%d",&machine_num);

    buffer[13] = "\0";
    sprintf(buffer,"machine.%d.log",machine_num);
    FILE* log = fopen(buffer,'w');
    
    fprintf(fp, "[SYSTEM]::LOGGING_SYSTEM_ONLINE\n");
    
    for(int i = 0; i < num_lines; i++){
        r = rand() % 10;
        switch(r){
            
            case 0:
                fprintf(fp, "[INFO]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::LOGON\n");
            case 1:
                fprintf(fp, "[INFO]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::LOGOFF\n");
            case 2:
                fprintf(fp, "[ERROR]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::INVALID_REQUEST\n");
            case 3:
                fprintf(fp, "[ERROR]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::CONNECTION_TIMEOUT\n");
            case 4:
                fprintf(fp, "[ERROR]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::SERVER_OVERLOADED\n");
            case 5:
                fprintf(fp, "[WARNING]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::INVALID_PASSWORD\n");
            case 6:
                fprintf(fp, "[WARNING]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::LOGON_ATTEMPTS_EXCEEDED\n");
            case 7:
                fprintf(fp, "[WARNING]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::OS_OUT_OF_DATE\n");
            case 8:
                fprintf(fp, "[INTRUSION]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::ACCOUNT_COMPROMISED\n");
            case 9:
                fprintf(fp, "[QUERY]::USER_");
                fputc(rand()%200, fp);
                fprintf(fp, "::QUERY_RECIEVED\n");
        }
    }
    fputc("\n",fp);
    fclose(fp);
}
