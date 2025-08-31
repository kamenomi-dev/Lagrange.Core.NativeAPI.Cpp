#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotLoginEvent : IEvent {
    CSharp_Int32            State{NULL};
    Common::ByteArrayNative Tag{};
    Common::ByteArrayNative Message{};

    bool IsSuccessful() const { return State == 0; }
};
} // namespace Lagrange::Definition::NativeModel::Event