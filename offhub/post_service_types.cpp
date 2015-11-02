/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "post_service_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace lj {

int _kEventTypeValues[] = {
  EventType::FORWARD,
  EventType::COMMENT,
  EventType::ZAN,
  EventType::ZAN_CANCEL
};
const char* _kEventTypeNames[] = {
  "FORWARD",
  "COMMENT",
  "ZAN",
  "ZAN_CANCEL"
};
const std::map<int, const char*> _EventType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(4, _kEventTypeValues, _kEventTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


PostServiceRequest::~PostServiceRequest() throw() {
}


void PostServiceRequest::__set_tid(const int32_t val) {
  this->tid = val;
}

void PostServiceRequest::__set_uid(const int32_t val) {
  this->uid = val;
}

void PostServiceRequest::__set_industry(const std::string& val) {
  this->industry = val;
}

void PostServiceRequest::__set_title(const std::string& val) {
  this->title = val;
}

void PostServiceRequest::__set_content(const std::string& val) {
  this->content = val;
}

void PostServiceRequest::__set_ctime(const int64_t val) {
  this->ctime = val;
}

void PostServiceRequest::__set_parent_tid(const int32_t val) {
  this->parent_tid = val;
}

void PostServiceRequest::__set_origin_tid(const int32_t val) {
  this->origin_tid = val;
}

void PostServiceRequest::__set_is_essence(const int32_t val) {
  this->is_essence = val;
}

void PostServiceRequest::__set_img(const std::string& val) {
  this->img = val;
}

const char* PostServiceRequest::ascii_fingerprint = "0A6B2A0D86A51712CCE4F59EFDE6D453";
const uint8_t PostServiceRequest::binary_fingerprint[16] = {0x0A,0x6B,0x2A,0x0D,0x86,0xA5,0x17,0x12,0xCC,0xE4,0xF5,0x9E,0xFD,0xE6,0xD4,0x53};

uint32_t PostServiceRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->tid);
          this->__isset.tid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uid);
          this->__isset.uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->industry);
          this->__isset.industry = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->title);
          this->__isset.title = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->content);
          this->__isset.content = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->ctime);
          this->__isset.ctime = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->parent_tid);
          this->__isset.parent_tid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->origin_tid);
          this->__isset.origin_tid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->is_essence);
          this->__isset.is_essence = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->img);
          this->__isset.img = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t PostServiceRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("PostServiceRequest");

  xfer += oprot->writeFieldBegin("tid", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->tid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("industry", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->industry);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("title", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->title);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("content", ::apache::thrift::protocol::T_STRING, 5);
  xfer += oprot->writeString(this->content);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ctime", ::apache::thrift::protocol::T_I64, 6);
  xfer += oprot->writeI64(this->ctime);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("parent_tid", ::apache::thrift::protocol::T_I32, 7);
  xfer += oprot->writeI32(this->parent_tid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("origin_tid", ::apache::thrift::protocol::T_I32, 8);
  xfer += oprot->writeI32(this->origin_tid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("is_essence", ::apache::thrift::protocol::T_I32, 9);
  xfer += oprot->writeI32(this->is_essence);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("img", ::apache::thrift::protocol::T_STRING, 10);
  xfer += oprot->writeString(this->img);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(PostServiceRequest &a, PostServiceRequest &b) {
  using ::std::swap;
  swap(a.tid, b.tid);
  swap(a.uid, b.uid);
  swap(a.industry, b.industry);
  swap(a.title, b.title);
  swap(a.content, b.content);
  swap(a.ctime, b.ctime);
  swap(a.parent_tid, b.parent_tid);
  swap(a.origin_tid, b.origin_tid);
  swap(a.is_essence, b.is_essence);
  swap(a.img, b.img);
  swap(a.__isset, b.__isset);
}

PostServiceRequest::PostServiceRequest(const PostServiceRequest& other0) {
  tid = other0.tid;
  uid = other0.uid;
  industry = other0.industry;
  title = other0.title;
  content = other0.content;
  ctime = other0.ctime;
  parent_tid = other0.parent_tid;
  origin_tid = other0.origin_tid;
  is_essence = other0.is_essence;
  img = other0.img;
  __isset = other0.__isset;
}
PostServiceRequest& PostServiceRequest::operator=(const PostServiceRequest& other1) {
  tid = other1.tid;
  uid = other1.uid;
  industry = other1.industry;
  title = other1.title;
  content = other1.content;
  ctime = other1.ctime;
  parent_tid = other1.parent_tid;
  origin_tid = other1.origin_tid;
  is_essence = other1.is_essence;
  img = other1.img;
  __isset = other1.__isset;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const PostServiceRequest& obj) {
  using apache::thrift::to_string;
  out << "PostServiceRequest(";
  out << "tid=" << to_string(obj.tid);
  out << ", " << "uid=" << to_string(obj.uid);
  out << ", " << "industry=" << to_string(obj.industry);
  out << ", " << "title=" << to_string(obj.title);
  out << ", " << "content=" << to_string(obj.content);
  out << ", " << "ctime=" << to_string(obj.ctime);
  out << ", " << "parent_tid=" << to_string(obj.parent_tid);
  out << ", " << "origin_tid=" << to_string(obj.origin_tid);
  out << ", " << "is_essence=" << to_string(obj.is_essence);
  out << ", " << "img=" << to_string(obj.img);
  out << ")";
  return out;
}


EventServiceRequest::~EventServiceRequest() throw() {
}


void EventServiceRequest::__set_type(const EventType::type val) {
  this->type = val;
}

void EventServiceRequest::__set_tid(const int32_t val) {
  this->tid = val;
}

const char* EventServiceRequest::ascii_fingerprint = "46A703A33337BED2F62F386FC66B2A5F";
const uint8_t EventServiceRequest::binary_fingerprint[16] = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

uint32_t EventServiceRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_type = false;
  bool isset_tid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast2;
          xfer += iprot->readI32(ecast2);
          this->type = (EventType::type)ecast2;
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->tid);
          isset_tid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_tid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t EventServiceRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("EventServiceRequest");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("tid", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->tid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(EventServiceRequest &a, EventServiceRequest &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.tid, b.tid);
}

EventServiceRequest::EventServiceRequest(const EventServiceRequest& other3) {
  type = other3.type;
  tid = other3.tid;
}
EventServiceRequest& EventServiceRequest::operator=(const EventServiceRequest& other4) {
  type = other4.type;
  tid = other4.tid;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const EventServiceRequest& obj) {
  using apache::thrift::to_string;
  out << "EventServiceRequest(";
  out << "type=" << to_string(obj.type);
  out << ", " << "tid=" << to_string(obj.tid);
  out << ")";
  return out;
}


FollowEvent::~FollowEvent() throw() {
}


void FollowEvent::__set_uid(const int32_t val) {
  this->uid = val;
}

void FollowEvent::__set_follower_uid(const int32_t val) {
  this->follower_uid = val;
}

const char* FollowEvent::ascii_fingerprint = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
const uint8_t FollowEvent::binary_fingerprint[16] = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};

uint32_t FollowEvent::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_uid = false;
  bool isset_follower_uid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uid);
          isset_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->follower_uid);
          isset_follower_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_follower_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FollowEvent::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("FollowEvent");

  xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("follower_uid", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->follower_uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(FollowEvent &a, FollowEvent &b) {
  using ::std::swap;
  swap(a.uid, b.uid);
  swap(a.follower_uid, b.follower_uid);
}

FollowEvent::FollowEvent(const FollowEvent& other5) {
  uid = other5.uid;
  follower_uid = other5.follower_uid;
}
FollowEvent& FollowEvent::operator=(const FollowEvent& other6) {
  uid = other6.uid;
  follower_uid = other6.follower_uid;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const FollowEvent& obj) {
  using apache::thrift::to_string;
  out << "FollowEvent(";
  out << "uid=" << to_string(obj.uid);
  out << ", " << "follower_uid=" << to_string(obj.follower_uid);
  out << ")";
  return out;
}

} // namespace
