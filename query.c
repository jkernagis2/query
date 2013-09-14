//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              query.c                                 //
//                                                                      //
//  Function definitions for the main program.  User chooses whether    //
//  to invoke the client or server interface                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//#include "client.h"
//#include "server.h"
#include "logs.c"

//#define SERVER_HARD_ADDRESS "127.0.0.1"
#define GREP_TEST_TARGET 

int main(){
    // Currently calls the log generator, then grep's it for
    
    int machine_num;
    
    printf("What machine am I? :::> ");
    scanf("%d",&machine_num);
    
    gen_logs(machine_num);
    
    return 0;

}
