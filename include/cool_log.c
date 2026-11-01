#include "cool_log.h"

#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

/* -- program constants -- */

#define CL_TIMESTAMP_BUF_SIZE 64
#define CL_FILE_BUF_SIZE 256
#define CL_LINE_BUF_SIZE 8192

/* -- private state for this file -- */

static Cl_Log_Mode log_mode = CL_MODE_NEW;
static Cl_Log_Level log_level = CL_LEVEL_DEBUG;
static FILE* log_file = NULL;

/* -- internal data -- */

static void get_timestamp(char* output_buf, size_t size, const char* fmt) {

    time_t now = time(NULL);
    struct tm tm_now = {0};

    localtime_s(&tm_now, &now);

    /* kill buffer on error */
    if (strftime(output_buf, size, fmt, &tm_now) == 0)
        output_buf[0] = '\0';

}

static const char* get_log_str(Cl_Log_Level level) {

    switch (level) {

        case CL_LEVEL_DEBUG: return "DEBUG";
        case CL_LEVEL_INFO: return "INFO";
        case CL_LEVEL_WARN: return "WARN";
        case CL_LEVEL_ERROR: return "ERROR";
        case CL_LEVEL_NONE: return "NONE";

        case CL_LEVEL_COUNT_:
            break;

    }

    return "UNKNOWN";

}

static const char* get_mode_str(Cl_Log_Mode mode) {

    switch (mode) {

        case CL_MODE_NEW: return "NEW";
        case CL_MODE_APPEND: return "APPEND";
        case CL_MODE_OVERWRITE: return "OVERWRITE";
        
        case CL_MODE_COUNT_:
            break;

    }

    return "UNKNOWN";

}

static void print_logn_internal(Cl_Log_Level level, const char* file, int line, const char* fmt, va_list args) {

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

    get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%H.%M.%S");
    len = snprintf(print_buf, CL_LINE_BUF_SIZE, "[%s]%s%s[%s] : ", get_log_str(level), file_buf, line_buf, timestamp_buf);

    if (len < 0 || (size_t) len >= CL_LINE_BUF_SIZE)
        return;

    vsnprintf(print_buf + len, CL_LINE_BUF_SIZE - len, fmt, args);

    fprintf(log_file, "%s\n", print_buf);
    fflush(log_file);

}

static void forced_log(const char* fmt, ...) {

    va_list args;
    
        va_start(args, fmt);
        print_logn_internal(CL_LEVEL_INFO, NULL, 0, fmt, args);

    va_end(args);

}

/* -- user usage -- */

void cl_set_log_mode(Cl_Log_Mode mode) {

    if (mode < 0 || mode >= CL_MODE_COUNT_)
        mode = CL_MODE_NEW;

    log_mode = mode;

}

void cl_set_log_level(Cl_Log_Level level) {

    if (level < 0 || level >= CL_LEVEL_COUNT_)
        level = CL_LEVEL_NONE;

    log_level = level;

}

bool cl_is_log_initialized(void) {

    return (log_file != NULL);

}

void cl_print_logn(Cl_Log_Level level, const char* file, int line, const char* fmt, ...) {

    if (!cl_is_log_initialized())
        return;

    if ((level < log_level) || (level >= CL_LEVEL_NONE))
        return;

    va_list args;

        va_start(args, fmt);
        print_logn_internal(level, file, line, fmt, args);
        
    va_end(args);

}

void cl_init_log(const char* filename) {

    if (filename == NULL)
        return;

    if (cl_is_log_initialized())
        return;

    char filename_buf[CL_LINE_BUF_SIZE] = {0};
    char timestamp_buf[CL_TIMESTAMP_BUF_SIZE] = {0};
    char* open_mode = "a"; /* append to prevent possible data loss */

    get_timestamp(timestamp_buf, CL_TIMESTAMP_BUF_SIZE, "%d.%m.%Y-%H.%M.%S");

    if (log_mode == CL_MODE_NEW) {

        snprintf(filename_buf, CL_LINE_BUF_SIZE, "%s_%s.cool_log", filename, timestamp_buf);
        open_mode = "w";

    } else if (log_mode == CL_MODE_APPEND || log_mode == CL_MODE_OVERWRITE) {

        snprintf(filename_buf, CL_LINE_BUF_SIZE, "%s.cool_log", filename);

    }

    if (log_mode == CL_MODE_NEW || log_mode == CL_MODE_OVERWRITE)
        open_mode = "w";
    else if (log_mode == CL_MODE_APPEND)
        open_mode = "a";

    log_file = fopen(filename_buf, open_mode);

    if (log_file == NULL) {

        fprintf(stderr, "couldnt create log file [%s]\n", filename_buf);
        return;

    }
    
    forced_log("hi!");
    forced_log("mode: %s", get_mode_str(log_mode));
    forced_log("level: %s\nlog started!", get_log_str(log_level));

}

void cl_end_log(void) {

    if (!cl_is_log_initialized())
        return;

    forced_log("log ended!\nbye bye!");

    fclose(log_file);
    log_file = NULL;

}