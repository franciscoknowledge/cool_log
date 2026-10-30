#include "cool_log.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

#define CL_TIMESTAMP_BUF_SIZE 64
#define CL_LINE_BUF_SIZE 4096

static FILE* log_file = NULL;

static void get_timestamp(char* output_buf, size_t size, const char* fmt) {

    time_t now = time(NULL);
    struct tm tm_now = {0};

    localtime_s(&tm_now, &now);

    /* kill buffer on error */
    if (strftime(output_buf, size, fmt, &tm_now) == 0)
        output_buf[0] = '\0';

}

bool cl_is_log_initialized(void) {

    return (log_file != NULL);

}

void cl_print_logn(const char* fmt, ...) {

    if (fmt == NULL)
        return;

    if (!cl_is_log_initialized())
        return;

    char timestamp_buf[CL_TIMESTAMP_BUF_SIZE] = {0};
    char line_buf[CL_LINE_BUF_SIZE] = {0};
    int len;
    va_list args;

    get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%H.%M.%S");
    len = snprintf(line_buf, CL_LINE_BUF_SIZE, "[%s] : ", timestamp_buf);

    /* potentially write errors here? */
    if (len < 0 || len >= CL_LINE_BUF_SIZE)
        return;

    va_start(args, fmt);

        vsnprintf(line_buf + len, CL_LINE_BUF_SIZE - len, fmt, args);

    va_end(args);

    fprintf(log_file, "%s\n", line_buf);
    fflush(log_file);

}

void cl_init_log(const char* filename) {

    if (filename == NULL)
        return;

    if (cl_is_log_initialized())
        return;

    char filename_buf[CL_LINE_BUF_SIZE] = {0};
    char timestamp_buf[CL_TIMESTAMP_BUF_SIZE] = {0};

    get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%d.%m.%Y-%H.%M.%S");

    snprintf(filename_buf, CL_LINE_BUF_SIZE, "%s_%s.cool_log", filename, timestamp_buf);
    log_file = fopen(filename_buf, "w");

    if (log_file == NULL) {

        fprintf(stderr, "couldnt create log file [%s]\n", filename_buf);
        return;

    }

    cl_print_logn("hi!\nlog started\n");

}

void cl_end_log() {

    if (!cl_is_log_initialized())
        return;

    cl_print_logn("log ended!\nbye bye!");

    fclose(log_file);
    log_file = NULL;

}