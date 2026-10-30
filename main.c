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

        cl_print_logn("couldnt open the test file!");
        cl_end_log();

        exit(EXIT_FAILURE);

    }

    while (fgets(read_buf, 64, so_peak) != NULL) {

        read_buf[strcspn(read_buf, "\n")] = '\0';
        if (strlen(read_buf) == 0)
            continue;
        
        cl_print_logn(read_buf);

    }

    cl_end_log();

}