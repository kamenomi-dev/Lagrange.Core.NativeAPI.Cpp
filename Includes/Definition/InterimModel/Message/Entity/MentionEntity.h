#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {
struct MentionEntity : IEntity {
    long long   Uin{NULL};
    std::u8string Display{};

    MentionEntity(
        NativeModel::Message::Entity::MentionEntity* e
    ) {
        Uin     = e->Uin;
        Display = e->Display;
    }
};
} // namespace Lagrange::Definition::InterimModel::Message::Entity