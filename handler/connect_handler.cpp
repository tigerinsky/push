#include "connect_handler.h"
#include "glog/logging.h"

namespace im {

void ConnectHandler::handle(const char* input_msg, int input_size,
                            const ::google::protobuf::Message** message) {
    _request.Clear();
    _response.Clear();
    if (!_request.ParseFromArray(input_msg, input_size)) {
        LOG(ERROR) << "connect handler parse error";
        _response.set_err_code(ILLEGAL_REQUEST);
        return;
    }
    _response.set_err_code(OK);
    *message = &_response;
}

}
