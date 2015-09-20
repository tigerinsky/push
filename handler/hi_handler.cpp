#include "hi_handler.h"

namespace im {

void HiHandler::handle(const char* input_msg, int input_size,
                       const ::google::protobuf::Message** message) {
    _request.Clear();
    _response.Clear();
    if (_request.ParseFromArray(input_msg, input_size)) {
        _response.set_msg(_request.msg());
    }
    *message = &_response;
}

}

