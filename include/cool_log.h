#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef enum {

    CL_MODE_NEW,
    CL_MODE_OVERWRITE,
    CL_MODE_APPEND,

    CL_MODE_COUNT_,

} Cl_Log_Mode;

typedef enum {

    CL_LEVEL_DEBUG,
    CL_LEVEL_INFO,
    CL_LEVEL_WARN,
    CL_LEVEL_ERROR,
    CL_LEVEL_NONE,

    CL_LEVEL_COUNT_,

} Cl_Log_Level;

/*
    must initialize with cl_init_log
    default log mode is CL_MODE_NEW
    default log level is CL_LEVEL_DEBUG
    make sure to set your log level and log mode before running cl_init_log!
*/

/* also functions as a NULL check for log_file */
bool cl_is_log_initialized(void);

/* must use before init ; set level for logging */
void cl_set_log_level(Cl_Log_Level level);

/* must use before init ; set log mode */
void cl_set_log_mode(Cl_Log_Mode mode);

/*
    must be called before doing anything with cool_log
    make sure to call set_log_mode and set_log_level beforehand
*/
void cl_init_log(const char* filename);

/* no newline needed, pass fmt string and arguments */
void cl_print_logn(Cl_Log_Level level, const char* file, int line, const char* fmt, ...);

/* please call on program end, but not strictly necessary */
void cl_end_log(void);

/* macros for logging */
#define cl_logn_debug(...) cl_print_logn(CL_LEVEL_DEBUG, __FILE__, __LINE__, "" __VA_ARGS__)
#define cl_logn_info(...) cl_print_logn(CL_LEVEL_INFO, __FILE__, __LINE__, "" __VA_ARGS__)
#define cl_logn_warn(...) cl_print_logn(CL_LEVEL_WARN, __FILE__, __LINE__, "" __VA_ARGS__)
#define cl_logn_error(...) cl_print_logn(CL_LEVEL_ERROR, __FILE__, __LINE__, "" __VA_ARGS__)