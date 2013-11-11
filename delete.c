#include <stdlib.h>
#include <string.h>
#include "request.h"

int main(int argc, char **argv) {
    send_req("s_delete",atoi(argv[1]),NULL);
    return 0;
}
