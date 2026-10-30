#include "cool_log.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

#define _CL_TIMESTAMP_BUF_SIZE 2000
#define _CL_PRINT_BUF_SIZE 4096

static char timestamp_buf[_CL_TIMESTAMP_BUF_SIZE] = {0}; /* do not modify outside timestamp functions please */
static char print_buf[_CL_PRINT_BUF_SIZE];

static struct tm _tm_now;
static time_t _timestamp;

static FILE* log_file = NULL;

static void _prepare_timestamp_stuff(void) {

    _timestamp = time(NULL);
    memset(timestamp_buf, 0, _CL_TIMESTAMP_BUF_SIZE);

    localtime_s(&_tm_now, &_timestamp);

}

static void fill_timestamp_buf_time(void) {

    _prepare_timestamp_stuff();
    strftime(timestamp_buf, _CL_TIMESTAMP_BUF_SIZE, "%H.%M.%S", &_tm_now);

}

static void fill_timestamp_buf_date(void) {

    _prepare_timestamp_stuff();
    strftime(timestamp_buf, _CL_TIMESTAMP_BUF_SIZE, "%d.%m.%Y;%H.%M.%S", &_tm_now);

}

/* also functions as a NULL check for log_file */
bool cl_is_log_initialized(void) {

    if (log_file == NULL)
        return false;

    return true;

}

/* no newline needed, pass fmt string and arguments */
void cl_print_logn(const char* fmt, ...) {

    if (!cl_is_log_initialized())
        return;

    fill_timestamp_buf_time();

    memset(print_buf, 0, _CL_PRINT_BUF_SIZE);
    int len = snprintf(print_buf, _CL_PRINT_BUF_SIZE, "[%s] : ", timestamp_buf);

    if (len < 0 || len >= _CL_PRINT_BUF_SIZE) {

        return;

    }

    va_list args;
    va_start(args, fmt);

        vsnprintf(print_buf + len, sizeof(print_buf) - len, fmt, args);

    va_end(args);

    fprintf_s(log_file, "%s\n", print_buf);

}

void cl_init_log(const char* filename) {

    if (cl_is_log_initialized())
        return;
    
    char filename_buf[_CL_PRINT_BUF_SIZE] = {0};
    snprintf(filename_buf, _CL_PRINT_BUF_SIZE, "%s", filename);

    fill_timestamp_buf_date();

    strcat_s(filename_buf, _CL_PRINT_BUF_SIZE, "_");
    strcat_s(filename_buf, _CL_PRINT_BUF_SIZE, timestamp_buf);
    strcat_s(filename_buf, _CL_PRINT_BUF_SIZE, ".cool_log");

    fopen_s(&log_file, filename_buf, "w");

    if (log_file == NULL) {

        fprintf(stderr, "couldnt create log file [%s]", filename_buf);
        return;

    }

    cl_print_logn("log started\n");

}

void cl_end_log() {

    if (!cl_is_log_initialized())
        return;

    cl_print_logn("log ended!\nbye bye!");
    fclose(log_file);

}