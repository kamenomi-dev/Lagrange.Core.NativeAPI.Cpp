#pragma once
#include "IEntity.h"

namespace Lagrange::InterimModel::Message::Entity {

class BotMessage;
struct MultiMsgEntity : IEntity {
    std::vector<NATIVE_MODEL Message::BotMessage*> Messages;
    std::u8string                                  ResId;

    MultiMsgEntity(
        NATIVE_MODEL Message::Entity::MultiMsgEntity* e
    ) {
        for (int index = 0; index < e->MessageCount; index++) {
            Messages.push_back((NATIVE_MODEL Message::BotMessage*)(e->Messages) + index);
        }
        ResId = e->ResId;
    }

    ~MultiMsgEntity() {
        for (auto& message : Messages) {
            DllExports::FreeMemory(message);
        }

        Messages.clear();
    }
};
} // namespace Lagrange::InterimModel::Message::Entity