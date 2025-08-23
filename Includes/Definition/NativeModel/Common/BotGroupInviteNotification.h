#pragma once
namespace Lagrange::Definition::NativeModel::Common {
struct BotGroupInviteNotification : public BotGroupNotificationBase {
    BotGroupNotificationState State{BotGroupNotificationState::Wait};
    CSharp_Int64              OperatorUin{NULL};
    ByteArrayNative           OperatorUid{};
    CSharp_Int64              InviterUin{NULL};
    ByteArrayNative           InviterUid{};
    CSharp_Boolean            IsFiltered{false};
};
} // namespace Lagrange::Definition::NativeModel::Common