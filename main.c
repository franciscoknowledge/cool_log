#include "include/cool_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    
    FILE* so_peak;
    char read_buf[64] = {0};

    cl_set_log_mode(CL_MODE_OVERWRITE);
    cl_set_log_level(CL_LEVEL_DEBUG);

    cl_init_log("cl_test");
    cl_logn_info("float: %f", 2.556);
    cl_logn_info("size_t: %zu", 199923999449999922);

    so_peak = fopen("hehe.peak", "r");
    if (so_peak == NULL) {

        cl_logn_error("couldnt open file!");
        cl_end_log();
        exit(EXIT_FAILURE);

    }

    while (fgets(read_buf, 64, so_peak) != NULL) {

        read_buf[strcspn(read_buf, "\n")] = '\0';
        if (strlen(read_buf) == 0)
            continue;

        cl_logn_info("%s", read_buf);
        
    }

    cl_logn_error("im printing this as an error, because i like this program so much and i dont want it to end");
    cl_logn_warn("but all good things must come to an end");
    cl_end_log();

}