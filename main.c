#include "include/cool_log.h"

#include <stdio.h>

int main(void) {

    cl_init_log("coolest_of_logs");
    cl_print_logn("testtest%d", 255);
    cl_end_log();

}