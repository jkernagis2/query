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
    gen_logs();
    system("grep GREP_TEST_TARGET machine.1.log > grepo");
    return 0;

}
