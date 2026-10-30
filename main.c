#include "include/cool_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    
    FILE* so_peak;
    char read_buf[64] = {0};

    cl_init_log("cl_test");

    so_peak = fopen("hehe.peak", "r");
    if (so_peak == NULL) {

        cl_end_log();

        exit(EXIT_FAILURE);

    }

    if (1) {

        cl_logn_debug("hey");
        cl_end_log();
        return 0;

    }

    while (fgets(read_buf, 64, so_peak) != NULL) {

        read_buf[strcspn(read_buf, "\n")] = '\0';
        if (strlen(read_buf) == 0)
            continue;
        
    }

    cl_end_log();

}