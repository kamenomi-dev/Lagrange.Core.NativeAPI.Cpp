#pragma once
#include "IEntity.h"

namespace Lagrange::InterimModel::Message::Entity {
struct TextEntity : IEntity {
    std::u8string Text{};

    TextEntity(
        NATIVE_MODEL Message::Entity::TextEntity* e
    ) {
        Text = e->Text;
    }
};
} // namespace Lagrange::InterimModel::Message::Entity