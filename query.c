//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              query.c                                 //
//                                                                      //
//  Function definitions for the main program.  User chooses whether    //
//  to invoke the client or server interface                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "logs.h"
#include "d_grep.h"
#include <stdlib.h>
#include <string.h>

#define SEARCH_STRING_1 "^INFO" // key
#define SEARCH_STRING_2 "^QUERY" // key
#define SEARCH_STRING_3 "D$" // value
#define SEARCH_STRING_4 "^USER_3" // value

int main(){
    // Currently calls the log generator, then grep's the generated log file for WARNING
    // Saves d_grep saves results in result.tmp

    int machine_num;
    int check = 0;
    
    gen_logs(1);
    gen_logs(2);
    gen_logs(3);
    gen_logs(4);
    
    // Now we do grep/gawk on the file
    d_grep("grep -k "SEARCH_STRING_1,1);
    d_grep("grep -k "SEARCH_STRING_1,2);
    d_grep("grep -k "SEARCH_STRING_1,3);
    d_grep("grep -k "SEARCH_STRING_1,4);

    check += verify_logs(1);
    
    d_grep("grep -k "SEARCH_STRING_2,1);
    d_grep("grep -k "SEARCH_STRING_2,2);
    d_grep("grep -k "SEARCH_STRING_2,3);
    d_grep("grep -k "SEARCH_STRING_2,4);
    
    check += verify_logs(2);
    
    d_grep("grep -v "SEARCH_STRING_3,1);
    d_grep("grep -v "SEARCH_STRING_3,2);
    d_grep("grep -v "SEARCH_STRING_3,3);
    d_grep("grep -v "SEARCH_STRING_3,4);
    
    check += verify_logs(3);
    
    d_grep("grep -v "SEARCH_STRING_4,1);
    d_grep("grep -v "SEARCH_STRING_4,2);
    d_grep("grep -v "SEARCH_STRING_4,3);
    d_grep("grep -v "SEARCH_STRING_4,4);
    
    check += verify_logs(4);
    
    if(check == 0){printf("Queries verified!\n");}
    
    combine();
    
    return 0;

}
