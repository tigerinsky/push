#ifndef PUSH_COMMON_H
#define PUSH_COMMON_H

#include "glog/logging.h"

#define LOG_FATAL LOG(FATAL)
#define LOG_ERROR LOG(ERROR)
#define LOG_WARN LOG(WARNING)
#define LOG_INFO LOG(INFO)
#define LOG_DEBUG VLOG(1)

#define TIME_DIFF(t1, t2) \
    ((t2.tv_sec - t1.tv_sec) * 100000 + t2.tv_usec - t1.tv_usec)

#endif
