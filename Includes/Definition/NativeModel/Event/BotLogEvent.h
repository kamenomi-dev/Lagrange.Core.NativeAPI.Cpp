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

    const char* LogLevelToEnumTag() {
        switch (Level) {
        case LogLevel::Trace:
            return "Trace";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Information:
            return "Information";
        case LogLevel::Warning:
            return "Warning";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Critical:
            return "Critical";
        default:
            return "Unknown";
        }
    }
};
} // namespace Lagrange::Definition::NativeModel::Event