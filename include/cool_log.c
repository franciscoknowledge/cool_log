#include "cool_log.h"

#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

#define CL_TIMESTAMP_BUF_SIZE 64
#define CL_FILE_BUF_SIZE 256
#define CL_LINE_BUF_SIZE 8192

static const char* const LOG_STRINGS[] = {
    [CL_LEVEL_DEBUG] = "DEBUG",
    [CL_LEVEL_INFO] = "INFO",
    [CL_LEVEL_WARN] = "WARN",
    [CL_LEVEL_ERROR] = "ERROR",
    [CL_LEVEL_NONE] = "NONE",
};

static Cl_Log_Level log_level = CL_LEVEL_DEBUG;
static FILE* log_file = NULL;

static void _get_timestamp(char* output_buf, size_t size, const char* fmt) {

    time_t now = time(NULL);
    struct tm tm_now = {0};

    localtime_s(&tm_now, &now);

    /* kill buffer on error */
    if (strftime(output_buf, size, fmt, &tm_now) == 0)
        output_buf[0] = '\0';

}

static const char* _get_log_str(Cl_Log_Level level) {

    if ((level < 0) || (level >= CL_LEVEL_COUNT) || (LOG_STRINGS[level] == NULL))
        return "UNKNOWN";

    return LOG_STRINGS[level];

}

static void _print_logn_internal(Cl_Log_Level level, const char* file, int line, const char* fmt, va_list args) {

    if (!cl_is_log_initialized())
        return;

    if (fmt == NULL)
        return;

    char file_buf[CL_FILE_BUF_SIZE] = "";
    char line_buf[CL_TIMESTAMP_BUF_SIZE] = "";

    char timestamp_buf[CL_TIMESTAMP_BUF_SIZE] = {0};
    char print_buf[CL_LINE_BUF_SIZE] = {0};

    int len;

    if (file != NULL)
        snprintf(file_buf, CL_FILE_BUF_SIZE, "[%s]", file);

    if (line > 0)
        snprintf(line_buf, CL_TIMESTAMP_BUF_SIZE, "[Line: %d]", line);

    _get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%H.%M.%S");
    len = snprintf(print_buf, CL_LINE_BUF_SIZE, "[%s]%s%s[%s] : ", _get_log_str(level), file_buf, line_buf, timestamp_buf);

    if (len < 0 || (size_t) len >= CL_LINE_BUF_SIZE)
        return;

    vsnprintf(print_buf + len, CL_LINE_BUF_SIZE - len, fmt, args);

    fprintf(log_file, "%s\n", print_buf);
    fflush(log_file);

}

static void _forced_log(const char* fmt, ...) {

    va_list args;
        va_start(args, fmt);
        _print_logn_internal(CL_LEVEL_INFO, NULL, 0, fmt, args);
    va_end(args);

}

bool cl_is_log_initialized(void) {

    return (log_file != NULL);

}

void cl_set_log_level(Cl_Log_Level level) {

    if (level < 0 || level >= CL_LEVEL_COUNT)
        level = CL_LEVEL_NONE;

    log_level = level;

}

void cl_print_logn(Cl_Log_Level level, const char* file, int line, const char* fmt, ...) {

    if (!cl_is_log_initialized())
        return;

    if ((level < log_level) || (level >= CL_LEVEL_NONE))
        return;

    va_list args;
        va_start(args, fmt);
        _print_logn_internal(level, file, line, fmt, args);
    va_end(args);

}

void cl_init_log(const char* filename) {

    if (filename == NULL)
        return;

    if (cl_is_log_initialized())
        return;

    char filename_buf[CL_LINE_BUF_SIZE] = {0};
    char timestamp_buf[CL_TIMESTAMP_BUF_SIZE] = {0};

    _get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%d.%m.%Y-%H.%M.%S");

    snprintf(filename_buf, CL_LINE_BUF_SIZE, "%s_%s.cool_log", filename, timestamp_buf);
    log_file = fopen(filename_buf, "w");

    if (log_file == NULL) {

        fprintf(stderr, "couldnt create log file [%s]\n", filename_buf);
        return;

    }
    
    _forced_log("hi!\nlog started!");

}

void cl_end_log(void) {

    if (!cl_is_log_initialized())
        return;

    _forced_log("log ended!\nbye bye!");

    fclose(log_file);
    log_file = NULL;

}