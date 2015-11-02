namespace php postoff 
namespace cpp lj 
namespace py lj 

include "meeting_broadcast.thrift"

struct PostServiceRequest{
    1: i32 tid,
    2: i32 uid,
    3: string industry,
    4: string title = "",
    5: string content,
    6: i64 ctime,
    7: i32 parent_tid = -1,
    8: i32 origin_tid = -1,
    9: i32 is_essence = 0,
    10: string img = "",
}

enum EventType {
    FORWARD = 0, //转发
    COMMENT = 1, //评论
    ZAN = 2, //点赞
    ZAN_CANCEL = 3, //取消点赞
}

struct EventServiceRequest {
    1: required EventType type,
    2: required i32 tid,
}

struct FollowEvent {
    1: required i32 uid,
    2: required i32 follower_uid,
}

service PostService{
    void SendNewPost(1:PostServiceRequest request)
    void SendNewEvent(1:EventServiceRequest request)
    void FollowNewEvent(1:FollowEvent event) 
    // 广播相关接口
    void SwitchUserOnOffline(1: meeting_broadcast.UserOnOfflineRequest request)
    void TalkVoiceTranslation(1: meeting_broadcast.TalkVoiceTranslationRequest request)
    void UserRoleChanged(1: meeting_broadcast.UserRoleChangedRequest request)
    void UserTalking(1: meeting_broadcast.UserTalkingRequest request)
    void ConnectOnOffline(1: meeting_broadcast.ConnectOnOfflineRequest request)
}

