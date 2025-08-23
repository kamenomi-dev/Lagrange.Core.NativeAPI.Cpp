#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotFriendRequestEvent : IEvent {
    Common::ByteArrayNative InitiatorUid{};
    CSharp_Int64            InitiatorUin{NULL};
    Common::ByteArrayNative Message{};
    Common::ByteArrayNative Source{};
};
} // namespace Lagrange::Definition::NativeModel::Event