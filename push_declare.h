#include "monitor/monitor.h"

DECLARE_STATISTIC_INT32(send_fail)
DECLARE_STATISTIC_INT32(send_success)
DECLARE_STATISTIC_INT32(connect)
DECLARE_STATISTIC_INT32(error_release)
DECLARE_STATISTIC_INT32(push_request)
DECLARE_ETERNAL_STATISTIC_INT64(all_send_fail)
DECLARE_ETERNAL_STATISTIC_INT64(all_send_success)
