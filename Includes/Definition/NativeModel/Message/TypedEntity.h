#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct TypedEntity {
    Entity::IEntity*   Entity{nullptr};
    Entity::EntityType Type{Entity::EntityType::ImageEntity};
};
} // namespace Lagrange::Definition::NativeModel::Message