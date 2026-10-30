#pragma once

#include <stdio.h>
#include <stdbool.h>

/* also functions as a NULL check for log_file */
bool cl_is_log_initialized(void);

/* must be called before doing anything with cool_log */
void cl_init_log(const char* filename);

/* no newline needed, pass fmt string and arguments */
void cl_print_logn(const char* fmt, ...);

/* please call on program end */
void cl_end_log(void);