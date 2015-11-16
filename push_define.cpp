#include "monitor/monitor.h"

DEFINE_STATISTIC_INT32(send_fail, kFlagSum, 10)
DEFINE_STATISTIC_INT32(send_success, kFlagSum, 10)
DEFINE_ETERNAL_STATISTIC_INT64(all_send_fail, kFlagSum)
DEFINE_ETERNAL_STATISTIC_INT64(all_send_success, kFlagSum)
DEFINE_STATISTIC_INT32(connect, kFlagCount)
DEFINE_STATISTIC_INT32(error_release, kFlagCount)
DEFINE_STATISTIC_INT32(push_request, kFlagBasic)
