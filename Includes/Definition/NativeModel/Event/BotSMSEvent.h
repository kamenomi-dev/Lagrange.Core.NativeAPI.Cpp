#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotSMSEvent : IEvent {
    Common::ByteArrayNative Url{};
    Common::ByteArrayNative Phone{};
};
} // namespace Lagrange::Definition::NativeModel::Event