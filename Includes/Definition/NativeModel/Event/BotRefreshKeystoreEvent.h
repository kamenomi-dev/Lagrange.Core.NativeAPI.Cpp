#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotRefreshKeystoreEvent : IEvent {
    Common::BotKeystore keystore{};
};
} // namespace Lagrange::Definition::NativeModel::Event