//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              logs.c                                  //
//  Function definitions for the generation of random log files.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "logs.h"


void log_event(int my_id, int num_machines, char* event, struct gossip_s* machine){
    char buf[50];
    int i;
    char* temp;
    
    memset(buf,'\0',strlen(buf));
    sprintf(buf, "%s%d%s", "./log/machine.",my_id,".log");
    FILE* fp = fopen(buf,"a");
    
    fprintf(fp,event);
    fputc(fp,':');
    for(i = 0;i < num_machines; i++){
        temp = inet_ntoa(machine.addr.sin_addr);
        fprintf(fp, "%s,%s,%d,%d,%d,%d;", (char*)machine[i].id, temp, (int)machine[i].counter,
                    (int)machine[i].time, (int)machine[i].p_crashed, (int)machine[i].has_left);
    }
    fputc(fp,'\n');
}
