#pragma once
namespace Lagrange::InterimModel::Message {
struct BotMessage {
    NATIVE_MODEL Common::IBotContact* Contact;
    NATIVE_MODEL Common::IBotContact* Receiver;
    NATIVE_MODEL Message::BotGroup Group;
    NATIVE_MODEL Message::MessageType Type;
    std::u8string                     Time;
    IncomingMessage                   Messages;

    BotMessage(
        NATIVE_MODEL Message::BotMessage e
    )
    : Messages(e) {
        Contact  = e.Contact;
        Receiver = e.Receiver;
        Group    = e.Group;
        Type     = e.Type;
        Time     = e.Time;
    }

    BotMessage(
        NATIVE_MODEL Message::BotMessage* e
    )
    : Messages(e) {
        Contact  = e->Contact;
        Receiver = e->Receiver;
        Group    = e->Group;
        Type     = e->Type;
        Time     = e->Time;
    }

    ~BotMessage() {
        DllExports::FreeMemory(Contact);
        DllExports::FreeMemory(Receiver);
    }
};
} // namespace Lagrange::InterimModel::Message