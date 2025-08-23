#pragma once
namespace Lagrange::Definition::NativeModel::Common {
struct BotGroupJoinNotification : public BotGroupNotificationBase {
    BotGroupNotificationState State{BotGroupNotificationState::Wait};
    CSharp_Int64              OperatorUin{NULL};
    ByteArrayNative           OperatorUid{};
    ByteArrayNative           Comment{};
    CSharp_Boolean            IsFiltered{false};
};
} // namespace Lagrange::Definition::NativeModel::Common