#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct MultiMsgEntity : IEntity {
    BotMessage*             Messages{nullptr};
    CSharp_Int32            MessageCount{NULL};
    Common::ByteArrayNative ResId{};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity