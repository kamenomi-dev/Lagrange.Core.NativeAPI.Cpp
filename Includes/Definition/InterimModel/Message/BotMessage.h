#pragma once
namespace Lagrange::Definition::InterimModel::Message {
struct BotMessage {
    NativeModel::Common::IBotContact* Contact;
    NativeModel::Common::IBotContact* Receiver;
    NativeModel::Message::BotGroup    Group;
    NativeModel::Message::MessageType Type;
    std::string                       Time;
    struct {
        NativeModel::Message::TypedEntity* Entities{nullptr};
        int                                EntityLength{NULL};
    } Messages;

    BotMessage(
        NativeModel::Message::BotMessage e
    ) {
        Contact               = e.Contact;
        Receiver              = e.Receiver;
        Group                 = e.Group;
        Type                  = e.Type;
        Time                  = e.Time;
        Messages.Entities     = e.Entities;
        Messages.EntityLength = e.EntityLength;
    }

    ~BotMessage() {
        DllExports::FreeMemory(Contact);
        DllExports::FreeMemory(Receiver);
    }
};
} // namespace Lagrange::Definition::InterimModel::Message