#pragma once
namespace Lagrange::Definition::NativeModel::Message {
enum class MessageType : CSharp_Int32 {
    Group = 0,
    Private,
    Temp
};

struct BotMessage {
    Common::IBotContact*    Contact{nullptr};
    Common::IBotContact*    Receiver{nullptr};
    BotGroup                Group{};
    MessageType             Type{MessageType::Group};
    Common::ByteArrayNative Time{};
    TypedEntity*            Entities{nullptr};
    CSharp_Int32            EntityLength{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Message