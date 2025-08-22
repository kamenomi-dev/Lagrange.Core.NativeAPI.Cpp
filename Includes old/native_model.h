#pragma once

#include <windows.h>
#include <string>

#include "submodule/cpp-base64/base64.h" // For ByteArrayNative::SerializeToBase64 function.

// 这里的命名空间是为符合接口导出规范而设计的
namespace LagrangeCore {
enum class StatusCode {
    Success = 0,
    UnInitialized,
    AlreadyInitialized,
    AlreadyStarted,
    InvalidIndex
};

using ContextIndex = int;
} // namespace LagrangeCore

namespace LagrangeCore::NativeModel {

namespace Common::Entity {
enum class BotGender : int {
    Unset   = 0,
    Male    = 1,
    Female  = 2,
    Unknown = 255
};
} // namespace Common::Entity

namespace Common {
struct ByteArrayNative {
    INT     Length{NULL};
    INT_PTR Data{NULL};

    operator std::string() { return std::string((const char*)Data, Length); }

    std::string SerializeToBase64() const { return base64_encode((const unsigned char*)Data, Length); }

    ByteArrayNative() = default;
    /// <summary>Warning: 注意，在调用这个函数时需要自己释放。</summary>
    ByteArrayNative(
        std::string& str
    ) {
        Length = (INT)str.length();
        Data   = (INT_PTR) new char[Length];
        memcpy((void*)Data, str.data(), Length);
    }

    void TryRelease() noexcept {
        try {
            if (Data) {
                ZeroMemory((void*)Data, Length);
            }
        } catch (...) {}

        Data   = NULL;
        Length = NULL;
    }
};

struct ByteArrayKVPNative {
    Common::ByteArrayNative Key{};
    Common::ByteArrayNative Value{};
};

using ByteArrayDictNative = ByteArrayNative;

struct BotConfig {
    BYTE Protocol          = 0b00000100;
    BOOL AutoReconnect     = true;
    BOOL UseIPv6Network    = false;
    BOOL GetOptimumServer  = true;
    UINT HighwayChunkSize  = 1024 * 1024;
    UINT HighwayConcurrent = 4;
    BOOL AutoReLogin       = true;
};

struct BotFriendCategory {
    INT                     Id = 0;
    Common::ByteArrayNative Name{};
    INT                     Count  = 0;
    INT                     SortId = 0;
};

struct BotKeystore {
    INT64                   Uin = 0;
    Common::ByteArrayNative Uid{};
    Common::ByteArrayNative Guid{};
    Common::ByteArrayNative AndroidId{};
    Common::ByteArrayNative Qimei{};
    Common::ByteArrayNative DeviceName{};
    Common::ByteArrayNative A2{};
    Common::ByteArrayNative A2Key{};
    Common::ByteArrayNative D2{};
    Common::ByteArrayNative D2Key{};
    Common::ByteArrayNative A1{};
    Common::ByteArrayNative A1Key{};
    Common::ByteArrayNative NoPicSig{};
    Common::ByteArrayNative TgtgtKey{};
    Common::ByteArrayNative Ksid{};
    Common::ByteArrayNative SuperKey{};
    Common::ByteArrayNative StKey{};
    Common::ByteArrayNative StWeb{};
    Common::ByteArrayNative St{};
    Common::ByteArrayNative WtSessionTicket{};
    Common::ByteArrayNative WtSessionTicketKey{};
    Common::ByteArrayNative RandomKey{};
    Common::ByteArrayNative SKey{};
    ByteArrayDictNative     PsKey{};

  public:
    ~BotKeystore() {
        ZeroMemory((void*)Uid.Data, Uid.Length);
        ZeroMemory((void*)Guid.Data, Guid.Length);
        ZeroMemory((void*)AndroidId.Data, AndroidId.Length);
        ZeroMemory((void*)Qimei.Data, Qimei.Length);
        ZeroMemory((void*)DeviceName.Data, DeviceName.Length);
        ZeroMemory((void*)A2.Data, A2.Length);
        ZeroMemory((void*)A2Key.Data, A2Key.Length);
        ZeroMemory((void*)D2.Data, D2.Length);
        ZeroMemory((void*)D2Key.Data, D2Key.Length);
        ZeroMemory((void*)A1.Data, A1.Length);
        ZeroMemory((void*)A1Key.Data, A1Key.Length);
        ZeroMemory((void*)NoPicSig.Data, NoPicSig.Length);
        ZeroMemory((void*)TgtgtKey.Data, TgtgtKey.Length);
        ZeroMemory((void*)Ksid.Data, Ksid.Length);
        ZeroMemory((void*)SuperKey.Data, SuperKey.Length);
        ZeroMemory((void*)StKey.Data, StKey.Length);
        ZeroMemory((void*)StWeb.Data, StWeb.Length);
        ZeroMemory((void*)St.Data, St.Length);
        ZeroMemory((void*)WtSessionTicket.Data, WtSessionTicket.Length);
        ZeroMemory((void*)WtSessionTicketKey.Data, WtSessionTicketKey.Length);
        ZeroMemory((void*)RandomKey.Data, RandomKey.Length);
        ZeroMemory((void*)SKey.Data, SKey.Length);
        ZeroMemory((void*)PsKey.Data, PsKey.Length);
    }

    operator bool() const {
        return Uin == 0 && Uid.Data == NULL && Guid.Data == NULL && AndroidId.Data == NULL && Qimei.Data == NULL
            && DeviceName.Data == NULL && A2.Data == NULL && A2Key.Data == NULL && D2.Data == NULL && D2Key.Data == NULL
            && A1.Data == NULL && A1Key.Data == NULL && NoPicSig.Data == NULL && TgtgtKey.Data == NULL
            && StKey.Data == NULL && StWeb.Data == NULL && St.Data == NULL && WtSessionTicket.Data == NULL
            && WtSessionTicketKey.Data == NULL && RandomKey.Data == NULL && SKey.Data == NULL;
    }
};
}; // namespace Common

namespace Message::Entity {
enum class EntityType {
    ImageEntity    = 0,
    MentionEntity  = 1,
    RecordEntity   = 2,
    ReplyEntity    = 3,
    VideoEntity    = 4,
    TextEntity     = 5,
    MultiMsgEntity = 6,
};

struct /* Interface */ IEntity {};

struct ImageEntity : public IEntity {
    Common::ByteArrayNative FileUrl;
    Common::ByteArrayNative FileName;
    Common::ByteArrayNative FileSha1;
    UINT                    FileSize = 0;
    Common::ByteArrayNative FileMd5;
    FLOAT                   ImageWidth  = 0;
    FLOAT                   ImageHeight = 0;
    INT                     SubType     = 0;
    Common::ByteArrayNative Summary;
    UINT                    RecordLength = 0;
};

struct MentionEntity : public IEntity {
    INT64                   Uin = 0;
    Common::ByteArrayNative Display;
};

struct RecordEntity : public IEntity {
    Common::ByteArrayNative FileUrl;
    Common::ByteArrayNative FileName;
    Common::ByteArrayNative FileSha1;
    UINT                    FileSize = 0;
    Common::ByteArrayNative FileMd5;
};

struct ReplyEntity : public IEntity {
    ULONG64 SrcUid      = 0;
    INT     SrcSequence = 0;
    INT_PTR Source      = 0;
    INT     SourceType  = 0;
};

struct VideoEntity : public IEntity {
    Common::ByteArrayNative FileUrl;
    Common::ByteArrayNative FileName;
    Common::ByteArrayNative FileSha1;
    UINT                    FileSize = 0;
    Common::ByteArrayNative FileMd5;
};

struct TextEntity : public IEntity {
    Common::ByteArrayNative Text;
};

struct MultiMsgEntity : public IEntity {
    INT_PTR                 Messages     = 0;
    INT                     MessageCount = 0;
    Common::ByteArrayNative ResId;
};

} // namespace Message::Entity

namespace Message {

enum MessageType : INT {
    Group     = 0,
    Private   = 1,
    Temporary = 2
};

struct BotFriendCategory {
    INT                     Id = 0;
    Common::ByteArrayNative Name;
    INT                     Count  = 0;
    INT                     SortId = 0;
};

struct BotFriend {
    INT64                     Uin = 0;
    Common::ByteArrayNative   Nickname;
    Common::ByteArrayNative   Uid;
    INT                       Age    = 0;
    Common::Entity::BotGender Gender = Common::Entity::BotGender::Unset;
    Common::ByteArrayNative   Remarks;
    Common::ByteArrayNative   PersonalSign;
    Common::ByteArrayNative   Qid;
    BotFriendCategory         Category;
};

struct BotStranger {
    INT64                     Uin = 0;
    Common::ByteArrayNative   Nickname;
    Common::ByteArrayNative   Uid;
    Common::ByteArrayNative   PersonalSign;
    Common::ByteArrayNative   Remark;
    ULONG64                   Level            = 0;
    Common::Entity::BotGender Gender           = Common::Entity::BotGender::Unset;
    INT64                     RegistrationTime = 0;
    INT64                     Birthday         = 0;
    ULONG64                   Age              = 0;
    Common::ByteArrayNative   Qid;
    INT64                     Source = 0;
};

struct BotGroup {
    INT64                   GroupUin = 0;
    Common::ByteArrayNative GroupName;
    INT                     MemberCount = 0;
    INT                     MaxMember   = 0;
    INT64                   CreateTime  = 0;
    Common::ByteArrayNative Description;
    Common::ByteArrayNative Question;
    Common::ByteArrayNative Announcement;
};

struct BotGroupMemeber {
    BotGroup                  BotGroup{};
    INT64                     Uin = 0;
    Common::ByteArrayNative   Uid;
    Common::ByteArrayNative   Nickname;
    INT                       Age        = 0;
    Common::Entity::BotGender Gender     = Common::Entity::BotGender::Unset;
    INT                       Permission = 0;
    INT                       GroupLevel = 0;
    Common::ByteArrayNative   MemberCard;
    Common::ByteArrayNative   SpecialTitle;
    Common::ByteArrayNative   JoinTime;
    Common::ByteArrayNative   LastMsgTime;
    Common::ByteArrayNative   ShutUpTimestamp;
};

struct BotMessage {
    INT_PTR                 Contact  = 0; // 需要手动释放
    INT_PTR                 Receiver = 0; // 需要手动释放
    BotGroup                Group{};
    MessageType             Type = MessageType::Group; // 这源码上写的默认值是 0，对应的是group
    Common::ByteArrayNative Time;
    INT_PTR                 Entities     = 0;
    INT                     EntityLength = 0;
};

struct TypedEntity {
    INT_PTR                     Entity = 0;                                        // 需要手动释放
    Message::Entity::EntityType Type   = Message::Entity::EntityType::ImageEntity; // default = 0;
};
} // namespace Message

namespace Event {

struct IEvent {};

struct EventArray {
    INT_PTR events;
    INT     count;
};

struct ReverseEventCount : public IEvent {
    int BotCaptchaEventCount                 = 0;
    int BotGroupInviteNotificationEventCount = 0;
    int BotGroupJoinNotificationEventCount   = 0;
    int BotGroupMemberDecreaseEventCount     = 0;
    int BotGroupNudgeEventCount              = 0;
    int BotLoginEventCount                   = 0;
    int BotLogEventCount                     = 0;
    int BotMessageEventCount                 = 0;
    int BotNewDeviceVerifyEventCount         = 0;
    int BotOnlineEventCount                  = 0;
    int BotQrCodeEventCount                  = 0;
    int BotQrCodeQueryEventCount             = 0;
    int BotRefreshKeystoreEventCount         = 0;
    int BotSMSEventCount                     = 0;

    bool operator==(
        ReverseEventCount target
    ) {
        return BotCaptchaEventCount == target.BotCaptchaEventCount
            && BotGroupInviteNotificationEventCount == target.BotGroupInviteNotificationEventCount
            && BotGroupJoinNotificationEventCount == target.BotGroupJoinNotificationEventCount
            && BotGroupMemberDecreaseEventCount == target.BotGroupMemberDecreaseEventCount
            && BotGroupNudgeEventCount == target.BotGroupNudgeEventCount
            && BotLoginEventCount == target.BotLoginEventCount && BotLogEventCount == target.BotLogEventCount
            && BotMessageEventCount == target.BotMessageEventCount
            && BotNewDeviceVerifyEventCount == target.BotNewDeviceVerifyEventCount
            && BotOnlineEventCount == target.BotOnlineEventCount && BotQrCodeEventCount == target.BotQrCodeEventCount
            && BotQrCodeQueryEventCount == target.BotQrCodeQueryEventCount
            && BotRefreshKeystoreEventCount == target.BotRefreshKeystoreEventCount
            && BotSMSEventCount == target.BotSMSEventCount;
    }
};

// Login

struct BotOnlineEvent : public IEvent {
    enum class Reasons : INT {
        Login     = 0,
        Reconnect = 1,
    };

    Reasons Reason = Reasons::Login;
};

struct BotLoginEvent : public IEvent {
    INT                     State = 0;
    Common::ByteArrayNative Tag;
    Common::ByteArrayNative Message;
};

struct BotSMSEvent : public IEvent {
    Common::ByteArrayNative Url;
    Common::ByteArrayNative Phone;
};

struct BotCaptchaEvent : public IEvent {
    Common::ByteArrayNative CaptchaUrl;
};

struct BotQrCodeEvent : public IEvent {
    Common::ByteArrayNative Url;
    Common::ByteArrayNative Image;
};

struct BotQrCodeQueryEvent : public IEvent {
    BYTE State = 0;
};

struct BotNewDeviceVerifyEvent : public IEvent {
    Common::ByteArrayNative Url;
};

struct BotRefreshKeystoreEvent : public IEvent {
    Common::BotKeystore Keystore{};
};

// Message

struct BotLogEvent : public IEvent {
    INT                     Level{NULL};
    Common::ByteArrayNative Tag;
    Common::ByteArrayNative Message;
};

struct BotMessageEvent : public IEvent {
    Message::BotMessage Message{};
};

// Group

struct BotGroupNudgeEvent : public IEvent {
    INT64 GroupUin    = 0;
    INT64 OperatorUin = 0;
    INT64 TargetUin   = 0;
};

}; // namespace Event

} // namespace LagrangeCore::NativeModel
