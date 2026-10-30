#include "cool_log.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stddef.h>

char _timestamp_buf[2000] = {0};
struct tm _tm_now;
time_t _timestamp;

FILE* log_file = NULL;

void _fill_timestamp_buf(void) {

    memset(_timestamp_buf, 0, 2000);

    localtime_s(&_tm_now, &_timestamp);
    strftime(_timestamp_buf, 2000, "%d|%m|%Y;%H|%M|%S", &_tm_now);

}

/* also functions as a NULL check for log_file */
bool cl_is_log_initialized(void) {

    if (log_file == NULL)
        return false;

    return true;

}

void cl_print_log(const char* fmt) {

    char timestamp_buf[2000] = {0};

}

void cl_init_log(const char* filename) {

    if (cl_is_log_initialized())
        return;

    char timestamp_buf[2000] = {0};
    char filename_buf[2000] = {0};

    time_t timestamp = time(NULL);
    struct tm tm_now;

    localtime_s(&tm_now, &timestamp);

    strftime(timestamp_buf, 2000, "%d|%m|%Y;%H|%M|%S", &tm_now);
    snprintf(filename_buf, 2000, "%s", filename);

    strcat_s(filename_buf, 2000, "_");
    strcat_s(filename_buf, 2000, timestamp_buf);
    strcat_s(filename_buf, 2000, ".cool_log");

    fopen_s(&log_file, filename_buf, "w");

    if (log_file == NULL) {

        fprintf(stderr, "couldnt create log file [%s]", filename_buf);
        return;

    }

}