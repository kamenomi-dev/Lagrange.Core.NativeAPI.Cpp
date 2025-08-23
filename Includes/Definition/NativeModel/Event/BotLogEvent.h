#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotLogEvent : IEvent {
    enum class LogLevel : CSharp_Int32 {
        Trace = 0,
        Debug,
        Information,
        Warning,
        Error,
        Critical
    };

    LogLevel                Level{LogLevel::Trace};
    Common::ByteArrayNative Tag{};
    Common::ByteArrayNative Message{};
};
} // namespace Lagrange::Definition::NativeModel::Event