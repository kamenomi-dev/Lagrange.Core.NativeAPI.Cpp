#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {
struct MultiMsgEntity : IEntity {
    std::vector<BotMessage> Messages;
    std::string             ResId;

    MultiMsgEntity(
        NativeModel::Message::Entity::MultiMsgEntity* e
    ) {
        for (int index = 0; index < e->MessageCount; index++) {
            Messages.push_back(*(NativeModel::Message::BotMessage*)(e->Messages + index * sizeof(void*)));
        }

        ResId = e->ResId;
    }
};
} // namespace Lagrange::Definition::InterimModel::Message::Entity