#pragma once
#include "IEntity.h"

namespace Lagrange::InterimModel::Message::Entity {
struct ReplyEntity : IEntity {
    enum class ReplyType : int {
        BotFriend      = 1,
        BotGroupMember = 2,
        BotStranger    = 3,
    };

    long long    SrcUid{NULL};
    long long    SrcSequence{NULL};
    NATIVE_MODEL Common::IBotContact* Source{nullptr};
    ReplyType                         SourceType{ReplyType ::BotFriend};

    ReplyEntity(
        NATIVE_MODEL Message::Entity::ReplyEntity* e
    ) {
        SrcUid      = e->SrcUid;
        SrcSequence = e->SrcSequence;
        Source      = e->Source;
        SourceType  = (ReplyType)e->SourceType;
    }

    ~ReplyEntity() { DllExports::FreeMemory(Source); }
};
} // namespace Lagrange::InterimModel::Message::Entity