#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotOnlineEvent : IEvent {
    enum Reasons : CSharp_Int32 {
        Login,
        Reconnect,
    };

    Reasons Reason{Reasons::Login};
};
} // namespace Lagrange::Definition::NativeModel::Event