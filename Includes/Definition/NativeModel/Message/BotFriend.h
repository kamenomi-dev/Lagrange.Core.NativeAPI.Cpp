#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct BotFriend : Common::IBotContact {
    CSharp_Int64              Uin{NULL};
    Common::ByteArrayNative   Nickname{};
    Common::ByteArrayNative   Uid{};
    CSharp_Int32              Age{NULL};
    Common::BotGender         Gender{Common::BotGender::Unset};
    Common::ByteArrayNative   Remarks{};
    Common::ByteArrayNative   PersonalSign{};
    Common::ByteArrayNative   Qid{};
    Common::BotFriendCategory Category{};
};
}; // namespace Lagrange::Definition::NativeModel::Message