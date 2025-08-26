#pragma once
namespace Lagrange::InterimModel::Message::Contact {
struct BotGroupMember {
    Contact::BotGroup BotGroup;
    long long         Uin;
    std::u8string     Uid;
    std::u8string     Nickname;
    int               Age;
    NATIVE_MODEL Common::BotGender Gender;
    NATIVE_MODEL Common::GroupMemberPermission Permission;
    int                                        GroupLevel;
    std::u8string                              MemberCard;
    std::u8string                              SpecialTitle;
    std::u8string                              JoinTime;
    std::u8string                              LastMsgTime;
    std::u8string                              ShutUpTimestamp;

    BotGroupMember(
        NATIVE_MODEL Message::BotGroupMember* member
    )
    : BotGroup(member->BotGroup),
      Uin(member->Uin),
      Uid(member->Uid),
      Nickname(member->Nickname),
      Age(member->Age),
      Gender(member->Gender),
      Permission(member->Permission),
      GroupLevel(member->GroupLevel),
      MemberCard(member->MemberCard),
      SpecialTitle(member->SpecialTitle),
      JoinTime(member->JoinTime),
      LastMsgTime(member->LastMsgTime),
      ShutUpTimestamp(member->ShutUpTimestamp) {}
};
} // namespace Lagrange::InterimModel::Message::Contact