/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef PostService_H
#define PostService_H

#include <thrift/TDispatchProcessor.h>
#include "post_service_types.h"

namespace lj {

class PostServiceIf {
 public:
  virtual ~PostServiceIf() {}
  virtual void SendNewPost(const PostServiceRequest& request) = 0;
  virtual void SendNewEvent(const EventServiceRequest& request) = 0;
  virtual void FollowNewEvent(const FollowEvent& event) = 0;
  virtual void SwitchUserOnOffline(const  ::lj::UserOnOfflineRequest& request) = 0;
  virtual void TalkVoiceTranslation(const  ::lj::TalkVoiceTranslationRequest& request) = 0;
  virtual void UserRoleChanged(const  ::lj::UserRoleChangedRequest& request) = 0;
  virtual void UserTalking(const  ::lj::UserTalkingRequest& request) = 0;
  virtual void ConnectOnOffline(const  ::lj::ConnectOnOfflineRequest& request) = 0;
};

class PostServiceIfFactory {
 public:
  typedef PostServiceIf Handler;

  virtual ~PostServiceIfFactory() {}

  virtual PostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(PostServiceIf* /* handler */) = 0;
};

class PostServiceIfSingletonFactory : virtual public PostServiceIfFactory {
 public:
  PostServiceIfSingletonFactory(const boost::shared_ptr<PostServiceIf>& iface) : iface_(iface) {}
  virtual ~PostServiceIfSingletonFactory() {}

  virtual PostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(PostServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<PostServiceIf> iface_;
};

class PostServiceNull : virtual public PostServiceIf {
 public:
  virtual ~PostServiceNull() {}
  void SendNewPost(const PostServiceRequest& /* request */) {
    return;
  }
  void SendNewEvent(const EventServiceRequest& /* request */) {
    return;
  }
  void FollowNewEvent(const FollowEvent& /* event */) {
    return;
  }
  void SwitchUserOnOffline(const  ::lj::UserOnOfflineRequest& /* request */) {
    return;
  }
  void TalkVoiceTranslation(const  ::lj::TalkVoiceTranslationRequest& /* request */) {
    return;
  }
  void UserRoleChanged(const  ::lj::UserRoleChangedRequest& /* request */) {
    return;
  }
  void UserTalking(const  ::lj::UserTalkingRequest& /* request */) {
    return;
  }
  void ConnectOnOffline(const  ::lj::ConnectOnOfflineRequest& /* request */) {
    return;
  }
};

typedef struct _PostService_SendNewPost_args__isset {
  _PostService_SendNewPost_args__isset() : request(false) {}
  bool request :1;
} _PostService_SendNewPost_args__isset;

class PostService_SendNewPost_args {
 public:

  static const char* ascii_fingerprint; // = "81CA9F8D0D7D08A14D960516230B2986";
  static const uint8_t binary_fingerprint[16]; // = {0x81,0xCA,0x9F,0x8D,0x0D,0x7D,0x08,0xA1,0x4D,0x96,0x05,0x16,0x23,0x0B,0x29,0x86};

  PostService_SendNewPost_args(const PostService_SendNewPost_args&);
  PostService_SendNewPost_args& operator=(const PostService_SendNewPost_args&);
  PostService_SendNewPost_args() {
  }

  virtual ~PostService_SendNewPost_args() throw();
  PostServiceRequest request;

  _PostService_SendNewPost_args__isset __isset;

  void __set_request(const PostServiceRequest& val);

  bool operator == (const PostService_SendNewPost_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_SendNewPost_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SendNewPost_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewPost_args& obj);
};


class PostService_SendNewPost_pargs {
 public:

  static const char* ascii_fingerprint; // = "81CA9F8D0D7D08A14D960516230B2986";
  static const uint8_t binary_fingerprint[16]; // = {0x81,0xCA,0x9F,0x8D,0x0D,0x7D,0x08,0xA1,0x4D,0x96,0x05,0x16,0x23,0x0B,0x29,0x86};


  virtual ~PostService_SendNewPost_pargs() throw();
  const PostServiceRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewPost_pargs& obj);
};


class PostService_SendNewPost_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_SendNewPost_result(const PostService_SendNewPost_result&);
  PostService_SendNewPost_result& operator=(const PostService_SendNewPost_result&);
  PostService_SendNewPost_result() {
  }

  virtual ~PostService_SendNewPost_result() throw();

  bool operator == (const PostService_SendNewPost_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_SendNewPost_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SendNewPost_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewPost_result& obj);
};


class PostService_SendNewPost_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_SendNewPost_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewPost_presult& obj);
};

typedef struct _PostService_SendNewEvent_args__isset {
  _PostService_SendNewEvent_args__isset() : request(false) {}
  bool request :1;
} _PostService_SendNewEvent_args__isset;

class PostService_SendNewEvent_args {
 public:

  static const char* ascii_fingerprint; // = "14CC3BAE3E6C0F21365D9CBC7848B83F";
  static const uint8_t binary_fingerprint[16]; // = {0x14,0xCC,0x3B,0xAE,0x3E,0x6C,0x0F,0x21,0x36,0x5D,0x9C,0xBC,0x78,0x48,0xB8,0x3F};

  PostService_SendNewEvent_args(const PostService_SendNewEvent_args&);
  PostService_SendNewEvent_args& operator=(const PostService_SendNewEvent_args&);
  PostService_SendNewEvent_args() {
  }

  virtual ~PostService_SendNewEvent_args() throw();
  EventServiceRequest request;

  _PostService_SendNewEvent_args__isset __isset;

  void __set_request(const EventServiceRequest& val);

  bool operator == (const PostService_SendNewEvent_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_SendNewEvent_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SendNewEvent_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewEvent_args& obj);
};


class PostService_SendNewEvent_pargs {
 public:

  static const char* ascii_fingerprint; // = "14CC3BAE3E6C0F21365D9CBC7848B83F";
  static const uint8_t binary_fingerprint[16]; // = {0x14,0xCC,0x3B,0xAE,0x3E,0x6C,0x0F,0x21,0x36,0x5D,0x9C,0xBC,0x78,0x48,0xB8,0x3F};


  virtual ~PostService_SendNewEvent_pargs() throw();
  const EventServiceRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewEvent_pargs& obj);
};


class PostService_SendNewEvent_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_SendNewEvent_result(const PostService_SendNewEvent_result&);
  PostService_SendNewEvent_result& operator=(const PostService_SendNewEvent_result&);
  PostService_SendNewEvent_result() {
  }

  virtual ~PostService_SendNewEvent_result() throw();

  bool operator == (const PostService_SendNewEvent_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_SendNewEvent_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SendNewEvent_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewEvent_result& obj);
};


class PostService_SendNewEvent_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_SendNewEvent_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_SendNewEvent_presult& obj);
};

typedef struct _PostService_FollowNewEvent_args__isset {
  _PostService_FollowNewEvent_args__isset() : event(false) {}
  bool event :1;
} _PostService_FollowNewEvent_args__isset;

class PostService_FollowNewEvent_args {
 public:

  static const char* ascii_fingerprint; // = "749EB551DE2BD2D51D4F65982507FADD";
  static const uint8_t binary_fingerprint[16]; // = {0x74,0x9E,0xB5,0x51,0xDE,0x2B,0xD2,0xD5,0x1D,0x4F,0x65,0x98,0x25,0x07,0xFA,0xDD};

  PostService_FollowNewEvent_args(const PostService_FollowNewEvent_args&);
  PostService_FollowNewEvent_args& operator=(const PostService_FollowNewEvent_args&);
  PostService_FollowNewEvent_args() {
  }

  virtual ~PostService_FollowNewEvent_args() throw();
  FollowEvent event;

  _PostService_FollowNewEvent_args__isset __isset;

  void __set_event(const FollowEvent& val);

  bool operator == (const PostService_FollowNewEvent_args & rhs) const
  {
    if (!(event == rhs.event))
      return false;
    return true;
  }
  bool operator != (const PostService_FollowNewEvent_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_FollowNewEvent_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_FollowNewEvent_args& obj);
};


class PostService_FollowNewEvent_pargs {
 public:

  static const char* ascii_fingerprint; // = "749EB551DE2BD2D51D4F65982507FADD";
  static const uint8_t binary_fingerprint[16]; // = {0x74,0x9E,0xB5,0x51,0xDE,0x2B,0xD2,0xD5,0x1D,0x4F,0x65,0x98,0x25,0x07,0xFA,0xDD};


  virtual ~PostService_FollowNewEvent_pargs() throw();
  const FollowEvent* event;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_FollowNewEvent_pargs& obj);
};


class PostService_FollowNewEvent_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_FollowNewEvent_result(const PostService_FollowNewEvent_result&);
  PostService_FollowNewEvent_result& operator=(const PostService_FollowNewEvent_result&);
  PostService_FollowNewEvent_result() {
  }

  virtual ~PostService_FollowNewEvent_result() throw();

  bool operator == (const PostService_FollowNewEvent_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_FollowNewEvent_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_FollowNewEvent_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_FollowNewEvent_result& obj);
};


class PostService_FollowNewEvent_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_FollowNewEvent_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_FollowNewEvent_presult& obj);
};

typedef struct _PostService_SwitchUserOnOffline_args__isset {
  _PostService_SwitchUserOnOffline_args__isset() : request(false) {}
  bool request :1;
} _PostService_SwitchUserOnOffline_args__isset;

class PostService_SwitchUserOnOffline_args {
 public:

  static const char* ascii_fingerprint; // = "C6F4184382604A0A9E6E14C21F610DCA";
  static const uint8_t binary_fingerprint[16]; // = {0xC6,0xF4,0x18,0x43,0x82,0x60,0x4A,0x0A,0x9E,0x6E,0x14,0xC2,0x1F,0x61,0x0D,0xCA};

  PostService_SwitchUserOnOffline_args(const PostService_SwitchUserOnOffline_args&);
  PostService_SwitchUserOnOffline_args& operator=(const PostService_SwitchUserOnOffline_args&);
  PostService_SwitchUserOnOffline_args() {
  }

  virtual ~PostService_SwitchUserOnOffline_args() throw();
   ::lj::UserOnOfflineRequest request;

  _PostService_SwitchUserOnOffline_args__isset __isset;

  void __set_request(const  ::lj::UserOnOfflineRequest& val);

  bool operator == (const PostService_SwitchUserOnOffline_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_SwitchUserOnOffline_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SwitchUserOnOffline_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SwitchUserOnOffline_args& obj);
};


class PostService_SwitchUserOnOffline_pargs {
 public:

  static const char* ascii_fingerprint; // = "C6F4184382604A0A9E6E14C21F610DCA";
  static const uint8_t binary_fingerprint[16]; // = {0xC6,0xF4,0x18,0x43,0x82,0x60,0x4A,0x0A,0x9E,0x6E,0x14,0xC2,0x1F,0x61,0x0D,0xCA};


  virtual ~PostService_SwitchUserOnOffline_pargs() throw();
  const  ::lj::UserOnOfflineRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SwitchUserOnOffline_pargs& obj);
};


class PostService_SwitchUserOnOffline_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_SwitchUserOnOffline_result(const PostService_SwitchUserOnOffline_result&);
  PostService_SwitchUserOnOffline_result& operator=(const PostService_SwitchUserOnOffline_result&);
  PostService_SwitchUserOnOffline_result() {
  }

  virtual ~PostService_SwitchUserOnOffline_result() throw();

  bool operator == (const PostService_SwitchUserOnOffline_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_SwitchUserOnOffline_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_SwitchUserOnOffline_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_SwitchUserOnOffline_result& obj);
};


class PostService_SwitchUserOnOffline_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_SwitchUserOnOffline_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_SwitchUserOnOffline_presult& obj);
};

typedef struct _PostService_TalkVoiceTranslation_args__isset {
  _PostService_TalkVoiceTranslation_args__isset() : request(false) {}
  bool request :1;
} _PostService_TalkVoiceTranslation_args__isset;

class PostService_TalkVoiceTranslation_args {
 public:

  static const char* ascii_fingerprint; // = "4DA7C99BE076F5A77231AF082F824680";
  static const uint8_t binary_fingerprint[16]; // = {0x4D,0xA7,0xC9,0x9B,0xE0,0x76,0xF5,0xA7,0x72,0x31,0xAF,0x08,0x2F,0x82,0x46,0x80};

  PostService_TalkVoiceTranslation_args(const PostService_TalkVoiceTranslation_args&);
  PostService_TalkVoiceTranslation_args& operator=(const PostService_TalkVoiceTranslation_args&);
  PostService_TalkVoiceTranslation_args() {
  }

  virtual ~PostService_TalkVoiceTranslation_args() throw();
   ::lj::TalkVoiceTranslationRequest request;

  _PostService_TalkVoiceTranslation_args__isset __isset;

  void __set_request(const  ::lj::TalkVoiceTranslationRequest& val);

  bool operator == (const PostService_TalkVoiceTranslation_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_TalkVoiceTranslation_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_TalkVoiceTranslation_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_TalkVoiceTranslation_args& obj);
};


class PostService_TalkVoiceTranslation_pargs {
 public:

  static const char* ascii_fingerprint; // = "4DA7C99BE076F5A77231AF082F824680";
  static const uint8_t binary_fingerprint[16]; // = {0x4D,0xA7,0xC9,0x9B,0xE0,0x76,0xF5,0xA7,0x72,0x31,0xAF,0x08,0x2F,0x82,0x46,0x80};


  virtual ~PostService_TalkVoiceTranslation_pargs() throw();
  const  ::lj::TalkVoiceTranslationRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_TalkVoiceTranslation_pargs& obj);
};


class PostService_TalkVoiceTranslation_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_TalkVoiceTranslation_result(const PostService_TalkVoiceTranslation_result&);
  PostService_TalkVoiceTranslation_result& operator=(const PostService_TalkVoiceTranslation_result&);
  PostService_TalkVoiceTranslation_result() {
  }

  virtual ~PostService_TalkVoiceTranslation_result() throw();

  bool operator == (const PostService_TalkVoiceTranslation_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_TalkVoiceTranslation_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_TalkVoiceTranslation_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_TalkVoiceTranslation_result& obj);
};


class PostService_TalkVoiceTranslation_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_TalkVoiceTranslation_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_TalkVoiceTranslation_presult& obj);
};

typedef struct _PostService_UserRoleChanged_args__isset {
  _PostService_UserRoleChanged_args__isset() : request(false) {}
  bool request :1;
} _PostService_UserRoleChanged_args__isset;

class PostService_UserRoleChanged_args {
 public:

  static const char* ascii_fingerprint; // = "C6F4184382604A0A9E6E14C21F610DCA";
  static const uint8_t binary_fingerprint[16]; // = {0xC6,0xF4,0x18,0x43,0x82,0x60,0x4A,0x0A,0x9E,0x6E,0x14,0xC2,0x1F,0x61,0x0D,0xCA};

  PostService_UserRoleChanged_args(const PostService_UserRoleChanged_args&);
  PostService_UserRoleChanged_args& operator=(const PostService_UserRoleChanged_args&);
  PostService_UserRoleChanged_args() {
  }

  virtual ~PostService_UserRoleChanged_args() throw();
   ::lj::UserRoleChangedRequest request;

  _PostService_UserRoleChanged_args__isset __isset;

  void __set_request(const  ::lj::UserRoleChangedRequest& val);

  bool operator == (const PostService_UserRoleChanged_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_UserRoleChanged_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_UserRoleChanged_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserRoleChanged_args& obj);
};


class PostService_UserRoleChanged_pargs {
 public:

  static const char* ascii_fingerprint; // = "C6F4184382604A0A9E6E14C21F610DCA";
  static const uint8_t binary_fingerprint[16]; // = {0xC6,0xF4,0x18,0x43,0x82,0x60,0x4A,0x0A,0x9E,0x6E,0x14,0xC2,0x1F,0x61,0x0D,0xCA};


  virtual ~PostService_UserRoleChanged_pargs() throw();
  const  ::lj::UserRoleChangedRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserRoleChanged_pargs& obj);
};


class PostService_UserRoleChanged_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_UserRoleChanged_result(const PostService_UserRoleChanged_result&);
  PostService_UserRoleChanged_result& operator=(const PostService_UserRoleChanged_result&);
  PostService_UserRoleChanged_result() {
  }

  virtual ~PostService_UserRoleChanged_result() throw();

  bool operator == (const PostService_UserRoleChanged_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_UserRoleChanged_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_UserRoleChanged_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserRoleChanged_result& obj);
};


class PostService_UserRoleChanged_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_UserRoleChanged_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserRoleChanged_presult& obj);
};

typedef struct _PostService_UserTalking_args__isset {
  _PostService_UserTalking_args__isset() : request(false) {}
  bool request :1;
} _PostService_UserTalking_args__isset;

class PostService_UserTalking_args {
 public:

  static const char* ascii_fingerprint; // = "F26FFAED7717A821CD18C2CE91CDB6BD";
  static const uint8_t binary_fingerprint[16]; // = {0xF2,0x6F,0xFA,0xED,0x77,0x17,0xA8,0x21,0xCD,0x18,0xC2,0xCE,0x91,0xCD,0xB6,0xBD};

  PostService_UserTalking_args(const PostService_UserTalking_args&);
  PostService_UserTalking_args& operator=(const PostService_UserTalking_args&);
  PostService_UserTalking_args() {
  }

  virtual ~PostService_UserTalking_args() throw();
   ::lj::UserTalkingRequest request;

  _PostService_UserTalking_args__isset __isset;

  void __set_request(const  ::lj::UserTalkingRequest& val);

  bool operator == (const PostService_UserTalking_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_UserTalking_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_UserTalking_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserTalking_args& obj);
};


class PostService_UserTalking_pargs {
 public:

  static const char* ascii_fingerprint; // = "F26FFAED7717A821CD18C2CE91CDB6BD";
  static const uint8_t binary_fingerprint[16]; // = {0xF2,0x6F,0xFA,0xED,0x77,0x17,0xA8,0x21,0xCD,0x18,0xC2,0xCE,0x91,0xCD,0xB6,0xBD};


  virtual ~PostService_UserTalking_pargs() throw();
  const  ::lj::UserTalkingRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserTalking_pargs& obj);
};


class PostService_UserTalking_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_UserTalking_result(const PostService_UserTalking_result&);
  PostService_UserTalking_result& operator=(const PostService_UserTalking_result&);
  PostService_UserTalking_result() {
  }

  virtual ~PostService_UserTalking_result() throw();

  bool operator == (const PostService_UserTalking_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_UserTalking_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_UserTalking_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserTalking_result& obj);
};


class PostService_UserTalking_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_UserTalking_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_UserTalking_presult& obj);
};

typedef struct _PostService_ConnectOnOffline_args__isset {
  _PostService_ConnectOnOffline_args__isset() : request(false) {}
  bool request :1;
} _PostService_ConnectOnOffline_args__isset;

class PostService_ConnectOnOffline_args {
 public:

  static const char* ascii_fingerprint; // = "9419B31C4621C3BA2CC1A9F33AB1717B";
  static const uint8_t binary_fingerprint[16]; // = {0x94,0x19,0xB3,0x1C,0x46,0x21,0xC3,0xBA,0x2C,0xC1,0xA9,0xF3,0x3A,0xB1,0x71,0x7B};

  PostService_ConnectOnOffline_args(const PostService_ConnectOnOffline_args&);
  PostService_ConnectOnOffline_args& operator=(const PostService_ConnectOnOffline_args&);
  PostService_ConnectOnOffline_args() {
  }

  virtual ~PostService_ConnectOnOffline_args() throw();
   ::lj::ConnectOnOfflineRequest request;

  _PostService_ConnectOnOffline_args__isset __isset;

  void __set_request(const  ::lj::ConnectOnOfflineRequest& val);

  bool operator == (const PostService_ConnectOnOffline_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const PostService_ConnectOnOffline_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_ConnectOnOffline_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_ConnectOnOffline_args& obj);
};


class PostService_ConnectOnOffline_pargs {
 public:

  static const char* ascii_fingerprint; // = "9419B31C4621C3BA2CC1A9F33AB1717B";
  static const uint8_t binary_fingerprint[16]; // = {0x94,0x19,0xB3,0x1C,0x46,0x21,0xC3,0xBA,0x2C,0xC1,0xA9,0xF3,0x3A,0xB1,0x71,0x7B};


  virtual ~PostService_ConnectOnOffline_pargs() throw();
  const  ::lj::ConnectOnOfflineRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_ConnectOnOffline_pargs& obj);
};


class PostService_ConnectOnOffline_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  PostService_ConnectOnOffline_result(const PostService_ConnectOnOffline_result&);
  PostService_ConnectOnOffline_result& operator=(const PostService_ConnectOnOffline_result&);
  PostService_ConnectOnOffline_result() {
  }

  virtual ~PostService_ConnectOnOffline_result() throw();

  bool operator == (const PostService_ConnectOnOffline_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const PostService_ConnectOnOffline_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostService_ConnectOnOffline_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const PostService_ConnectOnOffline_result& obj);
};


class PostService_ConnectOnOffline_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~PostService_ConnectOnOffline_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const PostService_ConnectOnOffline_presult& obj);
};

class PostServiceClient : virtual public PostServiceIf {
 public:
  PostServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  PostServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void SendNewPost(const PostServiceRequest& request);
  void send_SendNewPost(const PostServiceRequest& request);
  void recv_SendNewPost();
  void SendNewEvent(const EventServiceRequest& request);
  void send_SendNewEvent(const EventServiceRequest& request);
  void recv_SendNewEvent();
  void FollowNewEvent(const FollowEvent& event);
  void send_FollowNewEvent(const FollowEvent& event);
  void recv_FollowNewEvent();
  void SwitchUserOnOffline(const  ::lj::UserOnOfflineRequest& request);
  void send_SwitchUserOnOffline(const  ::lj::UserOnOfflineRequest& request);
  void recv_SwitchUserOnOffline();
  void TalkVoiceTranslation(const  ::lj::TalkVoiceTranslationRequest& request);
  void send_TalkVoiceTranslation(const  ::lj::TalkVoiceTranslationRequest& request);
  void recv_TalkVoiceTranslation();
  void UserRoleChanged(const  ::lj::UserRoleChangedRequest& request);
  void send_UserRoleChanged(const  ::lj::UserRoleChangedRequest& request);
  void recv_UserRoleChanged();
  void UserTalking(const  ::lj::UserTalkingRequest& request);
  void send_UserTalking(const  ::lj::UserTalkingRequest& request);
  void recv_UserTalking();
  void ConnectOnOffline(const  ::lj::ConnectOnOfflineRequest& request);
  void send_ConnectOnOffline(const  ::lj::ConnectOnOfflineRequest& request);
  void recv_ConnectOnOffline();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class PostServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<PostServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (PostServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_SendNewPost(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_SendNewEvent(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_FollowNewEvent(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_SwitchUserOnOffline(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_TalkVoiceTranslation(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UserRoleChanged(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UserTalking(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ConnectOnOffline(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  PostServiceProcessor(boost::shared_ptr<PostServiceIf> iface) :
    iface_(iface) {
    processMap_["SendNewPost"] = &PostServiceProcessor::process_SendNewPost;
    processMap_["SendNewEvent"] = &PostServiceProcessor::process_SendNewEvent;
    processMap_["FollowNewEvent"] = &PostServiceProcessor::process_FollowNewEvent;
    processMap_["SwitchUserOnOffline"] = &PostServiceProcessor::process_SwitchUserOnOffline;
    processMap_["TalkVoiceTranslation"] = &PostServiceProcessor::process_TalkVoiceTranslation;
    processMap_["UserRoleChanged"] = &PostServiceProcessor::process_UserRoleChanged;
    processMap_["UserTalking"] = &PostServiceProcessor::process_UserTalking;
    processMap_["ConnectOnOffline"] = &PostServiceProcessor::process_ConnectOnOffline;
  }

  virtual ~PostServiceProcessor() {}
};

class PostServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  PostServiceProcessorFactory(const ::boost::shared_ptr< PostServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< PostServiceIfFactory > handlerFactory_;
};

class PostServiceMultiface : virtual public PostServiceIf {
 public:
  PostServiceMultiface(std::vector<boost::shared_ptr<PostServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~PostServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<PostServiceIf> > ifaces_;
  PostServiceMultiface() {}
  void add(boost::shared_ptr<PostServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void SendNewPost(const PostServiceRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->SendNewPost(request);
    }
    ifaces_[i]->SendNewPost(request);
  }

  void SendNewEvent(const EventServiceRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->SendNewEvent(request);
    }
    ifaces_[i]->SendNewEvent(request);
  }

  void FollowNewEvent(const FollowEvent& event) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->FollowNewEvent(event);
    }
    ifaces_[i]->FollowNewEvent(event);
  }

  void SwitchUserOnOffline(const  ::lj::UserOnOfflineRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->SwitchUserOnOffline(request);
    }
    ifaces_[i]->SwitchUserOnOffline(request);
  }

  void TalkVoiceTranslation(const  ::lj::TalkVoiceTranslationRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->TalkVoiceTranslation(request);
    }
    ifaces_[i]->TalkVoiceTranslation(request);
  }

  void UserRoleChanged(const  ::lj::UserRoleChangedRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UserRoleChanged(request);
    }
    ifaces_[i]->UserRoleChanged(request);
  }

  void UserTalking(const  ::lj::UserTalkingRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UserTalking(request);
    }
    ifaces_[i]->UserTalking(request);
  }

  void ConnectOnOffline(const  ::lj::ConnectOnOfflineRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ConnectOnOffline(request);
    }
    ifaces_[i]->ConnectOnOffline(request);
  }

};

} // namespace

#endif
