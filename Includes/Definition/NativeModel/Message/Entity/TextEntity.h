#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct TextEntity : IEntity {
    Common::ByteArrayNative Text{};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity