#pragma once
#include "IEntity.h"

namespace Lagrange::InterimModel::Message::Entity {
struct MentionEntity : IEntity {
    long long     Uin{NULL};
    std::u8string Display{};

    MentionEntity(
        NATIVE_MODEL Message::Entity::MentionEntity* e
    ) {
        Uin     = e->Uin;
        Display = e->Display;
    }
};
} // namespace Lagrange::InterimModel::Message::Entity