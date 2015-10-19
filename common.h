#ifndef PUSH_COMMON_H
#define PUSH_COMMON_H

#include <sys/time.h>
#include "glog/logging.h"

#define LOG_FATAL LOG(FATAL)
#define LOG_ERROR LOG(ERROR)
#define LOG_WARN LOG(WARNING)
#define LOG_INFO LOG(INFO)
#define LOG_DEBUG VLOG(1)

#define TIME_DIFF(t1, t2) \
    ((t2.tv_sec - t1.tv_sec) * 100000 + t2.tv_usec - t1.tv_usec)

struct timeval nowtime();

#define run_within_time(ms) \
    for (struct timeval now = nowtime(); TIME_DIFF(now, nowtime()) * 1000 < ms;) 

char* md5(const char* data, int size, char* output);

char* sha1(const char* data, int size, char* output);

#endif



