#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotCaptchaEvent : IEvent {
    Common::ByteArrayNative CaptchaUrl{};
};
} // namespace Lagrange::Definition::NativeModel::Event