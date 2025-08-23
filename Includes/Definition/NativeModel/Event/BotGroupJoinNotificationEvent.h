#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotGroupJoinNotificationEvent : IEvent {
    Common::BotGroupJoinNotification Notification{};
};
} // namespace Lagrange::Definition::NativeModel::Event