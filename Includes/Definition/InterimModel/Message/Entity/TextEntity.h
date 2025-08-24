#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {
struct TextEntity : IEntity {
    std::u8string Text{};

    TextEntity(
        NativeModel::Message::Entity::TextEntity* e
    ) {
        Text = e->Text;
    }
};
} // namespace Lagrange::Definition::InterimModel::Message::Entity