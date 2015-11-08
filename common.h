#ifndef PUSH_COMMON_H
#define PUSH_COMMON_H

#include <sys/time.h>
#include <stdarg.h>                              // va_list
#include <string>                                // std::string
#include "glog/logging.h"

#define LOG_FATAL LOG(FATAL)
#define LOG_ERROR LOG(ERROR)
#define LOG_WARN LOG(WARNING)
#define LOG_INFO LOG(INFO)
#define LOG_DEBUG VLOG(1)

#define TIME_DIFF(t1, t2) \
    ((t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec)

struct timeval nowtime();

#define run_within_time(ms) \
    for (struct timeval now = nowtime(); TIME_DIFF(now, nowtime()) / 1000 < ms;) 

char* md5(const char* data, int size, char* output);

char* sha1(const char* data, int size, char* output);

std::string string_printf(const char* format, ...);

// Write |format| and associated arguments into |output|
// Returns 0 on success, -1 otherwise.
int string_printf(std::string* output, const char* fmt, ...);

// Write |format| and associated arguments in form of va_list into |output|.
// Returns 0 on success, -1 otherwise.
int string_vprintf(std::string* output, const char* format, va_list args);

// Append |format| and associated arguments to |output|
// Returns 0 on success, -1 otherwise.
int string_appendf(std::string* output, const char* format, ...);

// Append |format| and associated arguments in form of va_list to |output|.
// Returns 0 on success, -1 otherwise.
int string_vappendf(std::string* output, const char* format, va_list args);

#endif



