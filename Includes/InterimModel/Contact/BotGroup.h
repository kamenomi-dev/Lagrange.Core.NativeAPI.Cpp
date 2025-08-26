#pragma once
namespace Lagrange::InterimModel::Message::Contact {
struct BotGroup {
    long long     GroupUin{NULL};
    std::u8string GroupName{};
    int           MemberCount{NULL};
    int           MaxMember{NULL};
    long long     CreateTime{NULL};
    std::u8string Description{};
    std::u8string Question{};
    std::u8string Announcement{};

    BotGroup(
        NATIVE_MODEL Message::BotGroup group
    )
    : GroupUin(group.GroupUin),
      GroupName(group.GroupName),
      MemberCount(group.MemberCount),
      MaxMember(group.MaxMember),
      CreateTime(group.CreateTime),
      Description(group.Description),
      Question(group.Question),
      Announcement(group.Announcement) {}
};
} // namespace Lagrange::InterimModel::Message::Contact