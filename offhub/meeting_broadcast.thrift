namespace cpp lj
namespace php postoff
namespace py lj

// 在线/离线广播
struct UserOnOfflineRequest {
    1: i32 uid,
    2: i32 operate_type,    // 0:离开会议 1:进入会议
    3: i32 meeting_id,
    4: i64 ctime,
}

// 文字翻译
struct TalkVoiceTranslationRequest {
    1: i32 talk_id,         // 对话id
    2: string content,      // 对话内容
    3: i32 meeting_id,      // 会议id
    4: i64 ctime,
}

// 身份变化
struct UserRoleChangedRequest {
    1: i32 uid,             // 用户id
    2: i32 role,            // 角色 1.创建者, 2.管理员 3.嘉宾 4.普通用户
    3: i32 meeting_id,      // 会议id
    4: i64 ctime,
}

// 发言
// - 聊天信息
struct TalkingInfo {
    1: i32 talk_id,             // 对话id
    2: i32 display_position,    // 展现位置 1:会议栏 2:讨论栏
    3: i32 info_type,           // 聊天类型 1:文字 2:图片 3:语音
    4: string content,          // 内容 如果是语音则为url
    5: i64 ctime,               // 聊天时间
    6: i32 height,              // 图片宽
    7: i32 width,               // 图片高
    8: string audio_format,     // 音频格式
    9: i32 audio_duration,      // 音频时长/s
    10: bool is_question,       // 是否是提问
}

// - 用户信息
struct UserInfo {
    1: i32 uid,                 // 用户id
    2: i32 role,                // 用户角色 1-创建者，2-管理员，3-嘉宾，4-普通用户
    3: string avatar,           // 用户头像
    4: string sname,            // 用户昵称
}

struct UserTalkingRequest {
    1: TalkingInfo talking_info,    // 聊天信息
    2: UserInfo user_info,          // 用户信息
    3: i32 meeting_id,              // 会议id
}

// 连接/断线
struct ConnectOnOfflineRequest {
    1: i64 connect_id,          // 连接id
    2: i32 status,              // 状态 0: 连接 1:断开
}
