#ifndef PUSH_FLAG_H
#define PUSH_FLAG_H

#include "google/gflags.h"

namespace im {

DECLARE_int32(port);
DECLARE_string(offhub_host);
DECLARE_int32(offhub_port);
DECLARE_bool(enable_conn_notify);
DECLARE_bool(enable_token_verify);
DECLARE_bool(enable_sign_verify);

}

#endif
