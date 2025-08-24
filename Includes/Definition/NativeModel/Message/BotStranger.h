#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct BotStranger : Common::IBotContact {
    CSharp_Int64            Uin{NULL};
    Common::ByteArrayNative Nickname{};
    Common::ByteArrayNative Uid{};
    Common::ByteArrayNative PersonalSign{};
    Common::ByteArrayNative Remark{};
    CSharp_UInt64           Level{NULL};
    Common::BotGender       Gender{Common::BotGender::Unset};
    CSharp_Int64            RegistrationTime{NULL};
    CSharp_Int64            Birthday{NULL};
    CSharp_UInt64           Age{NULL};
    Common::ByteArrayNative QID{};
    CSharp_Int64            Source{NULL};
    Common::ByteArrayNative Country{};
    Common::ByteArrayNative City{};
    Common::ByteArrayNative School{};
};
} // namespace Lagrange::Definition::NativeModel::Message