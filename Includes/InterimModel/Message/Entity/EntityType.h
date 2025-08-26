#pragma once
namespace Lagrange::Definition::InterimModel::Message::Entity {
enum class EntityType : CSharp_Int32 {
    ImageEntity    = 0,
    MentionEntity  = 1,
    RecordEntity   = 2,
    ReplyEntity    = 3,
    VideoEntity    = 4,
    TextEntity     = 5,
    MultiMsgEntity = 6,
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity