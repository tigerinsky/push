#ifndef PUSH_COMMUNICATION_H
#define PUSH_COMMUNICATION_H

#include "msg.pb.h"

namespace im {

int send(int fd, 
         const std::string& name,
         const ::google::protobuf::Message& message,
         uint64_t mid=0);

int receive(int fd, Msg* message);
}

#endif
