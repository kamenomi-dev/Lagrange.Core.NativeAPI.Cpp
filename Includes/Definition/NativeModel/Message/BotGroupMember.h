#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct BotGroupMember : Common::IBotContact {
    BotGroup                      BotGroup{};
    CSharp_Int64                  Uin{NULL};
    Common::ByteArrayNative       Uid{};
    Common::ByteArrayNative       Nickname{};
    CSharp_Int32                  Age{NULL};
    Common::BotGender             Gender{NULL};
    Common::GroupMemberPermission Permission{NULL};
    CSharp_Int32                  GroupLevel{NULL};
    Common::ByteArrayNative       MemberCard{};
    Common::ByteArrayNative       SpecialTitle{};
    Common::ByteArrayNative       JoinTime{};
    Common::ByteArrayNative       LastMsgTime{};
    Common::ByteArrayNative       ShutUpTimestamp{};
};
} // namespace Lagrange::Definition::NativeModel::Message