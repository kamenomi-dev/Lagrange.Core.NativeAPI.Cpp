#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct MentionEntity : IEntity {
    CSharp_Int64            Uin{NULL};
    Common::ByteArrayNative Display{};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity