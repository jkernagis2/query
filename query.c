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

int main(){
    gen_logs();
    system("grep WARNING machine.1.log > grepo");
    return 0;

}
