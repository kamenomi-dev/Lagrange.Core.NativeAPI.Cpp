#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {

class BotMessage;
struct MultiMsgEntity : IEntity {
    std::vector<NativeModel::Message::BotMessage> Messages;
    std::u8string                                 ResId;

    MultiMsgEntity(
        NativeModel::Message::Entity::MultiMsgEntity* e
    ) {
        for (int index = 0; index < e->MessageCount; index++) {
            Messages.push_back(*(NativeModel::Message::BotMessage*)(e->Messages + index));
        }
        ResId = e->ResId;
    }

    ~MultiMsgEntity() {
        for (auto& message : Messages) {
            DllExports::FreeMemory(&message);
        }

        Messages.clear();
    }
};
} // namespace Lagrange::Definition::InterimModel::Message::Entity