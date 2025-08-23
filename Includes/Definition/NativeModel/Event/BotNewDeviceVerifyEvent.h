#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotNewDeviceVerifyEvent : IEvent {
    Common::ByteArrayNative Url{};
};
} // namespace Lagrange::Definition::NativeModel::Event