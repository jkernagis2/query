#include <stdlib.h>
#include <string.h>
#include "request.h"

int main(int argc, char **argv) {
    send_req("s_update",atoi(argv[1]),argv[2]);
    return 0;
}
