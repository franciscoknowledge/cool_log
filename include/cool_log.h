#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef enum {

    CL_LEVEL_DEBUG,
    CL_LEVEL_INFO,
    CL_LEVEL_WARN,
    CL_LEVEL_ERROR,
    CL_LEVEL_NONE,

    CL_LEVEL_COUNT,

} Cl_Log_Level;

/* also functions as a NULL check for log_file */
bool cl_is_log_initialized(void);

/* set level for logging */
void cl_set_log_level(Cl_Log_Level level);

/* must be called before doing anything with cool_log */
void cl_init_log(const char* filename);

/* no newline needed, pass fmt string and arguments */
void cl_print_logn(Cl_Log_Level level, const char* file, int line, const char* fmt, ...);

/* please call on program end */
void cl_end_log(void);

/* macros for logging */
#define cl_logn_debug(...) cl_print_logn(CL_LEVEL_DEBUG, __FILE__, __LINE__, "" __VA_ARGS__)