#include <stdio.h>
#include <unistd.h>

/* Uncommet below line enable logs */
// #define ENABLE_LOGS 1
#include "lib/srec.h"


int main() {
    parse_srec_status result = parse_srec_fn("example.srec");
    if (result == SREC_PARSE_SUCCESS) {
        printf("SREC parsing successful.\n");
    } else {
        printf("SREC parsing failed.\n");
    }
}
