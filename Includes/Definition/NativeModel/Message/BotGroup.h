#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct BotGroup {
    CSharp_Int64            GroupUin{NULL};
    Common::ByteArrayNative GroupName{};
    CSharp_Int32            MemberCount{NULL};
    CSharp_Int32            MaxMember{NULL};
    CSharp_Int64            CreateTime{NULL};
    Common::ByteArrayNative Description{};
    Common::ByteArrayNative Question{};
    Common::ByteArrayNative Announcement{};

};
} // namespace Lagrange::Definition::NativeModel::Message