#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotQrCodeEvent : IEvent {
    Common::ByteArrayNative Url{};
    Common::ByteArrayNative Image{};
};
} // namespace Lagrange::Definition::NativeModel::Event