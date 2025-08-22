#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotMessageEvent : IEvent {
    Message::BotMessage Message{};
};
} // namespace Lagrange::Definition::NativeModel::Event