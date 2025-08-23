#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotGroupInviteNotificationEvent : IEvent{
    Common::BotGroupInviteNotification Notification{};
};
}