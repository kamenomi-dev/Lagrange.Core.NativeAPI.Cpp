#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotLoginEvent : IEvent {
    CSharp_Int32            State{NULL};
    Common::ByteArrayNative Tag{};
    Common::ByteArrayNative Message{};
};
} // namespace Lagrange::Definition::NativeModel::Event