#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct ReplyEntity : IEntity {
    enum class ReplyType : CSharp_Int32 {
        BotFriend      = 1,
        BotGroupMember = 2,
        BotStranger    = 3,
    };

    CSharp_UInt64        SrcUid{NULL};
    CSharp_UInt64        SrcSequence{NULL};
    Common::IBotContact* Source{nullptr};
    ReplyType            SourceType{ReplyType ::BotFriend};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity