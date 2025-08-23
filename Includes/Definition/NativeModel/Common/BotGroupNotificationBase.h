#pragma once
namespace Lagrange::Definition::NativeModel::Common {
enum class BotGroupNotificationType : CSharp_Int32 {
    Join       = 1,
    SetAdmin   = 3,
    KickOther  = 6,
    KickSelf   = 7,
    Exit       = 13,
    UnsetAdmin = 16,
    Invite     = 22
};

enum class BotGroupNotificationState : CSharp_Int32 {
    Wait = 1,
    Accept,
    Reject,
    Ignore
};

struct BotGroupNotificationBase {
    CSharp_Int64             GroupUin{NULL};
    CSharp_UInt64            Sequence{NULL};
    BotGroupNotificationType Type{BotGroupNotificationType ::Join};
    CSharp_Int64             TargetUin{NULL};
    ByteArrayNative          TargetUid{};
};
} // namespace Lagrange::Definition::NativeModel::Common
