#pragma once
namespace Lagrange::InterimModel::Message {
struct BotGroupMessage {
    Contact::BotGroup       Group;
    Contact::BotGroupMember Sender;
    IncomingMessage         Messages;
    std::u8string           Time;

    BotGroupMessage(
        NATIVE_MODEL Message::BotMessage message
    )
    : Group(message.Group),
      Sender((NATIVE_MODEL Message::BotGroupMember*)message.Contact),
      Messages(message),
      Time(message.Time) {
        if (message.Type != NATIVE_MODEL Message::MessageType::Group) {
            throw std::runtime_error("Cannot convert NATIVE_MODEL Message::BotMessage to BotGroupMessage");
        };
    }
};
} // namespace Lagrange::InterimModel::Message