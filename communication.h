#ifndef PUSH_COMMUNICATION_H
#define PUSH_COMMUNICATION_H

#include "google/protobuf/message.h"
#include "protocol.h"

namespace im {

int send(int fd, 
         const std::string& name,
         const ::google::protobuf::Message& message,
         uint64_t mid=0);

int receive(int fd, message_t* message);
}

#endif
