#pragma once

#include <stdio.h>
#include <stdbool.h>

bool cl_is_log_initialized(void);
void cl_init_log(const char* filename);

void cl_print_logn(const char* fmt, ...);
void cl_end_log(void);