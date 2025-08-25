#pragma once
namespace Lagrange::Definition::InterimModel::Message {
struct BotMessage {
    NativeModel::Common::IBotContact* Contact;
    NativeModel::Common::IBotContact* Receiver;
    NativeModel::Message::BotGroup    Group;
    NativeModel::Message::MessageType Type;
    std::u8string                     Time;
    IncomingMessage                   Messages;

    BotMessage(
        NativeModel::Message::BotMessage e
    )
    : Messages(e) {
        Contact  = e.Contact;
        Receiver = e.Receiver;
        Group    = e.Group;
        Type     = e.Type;
        Time     = e.Time;
    }

    ~BotMessage() {
        DllExports::FreeMemory(Contact);
        DllExports::FreeMemory(Receiver);
    }
};
} // namespace Lagrange::Definition::InterimModel::Message