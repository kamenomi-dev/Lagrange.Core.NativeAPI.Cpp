#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotGroupNudgeEvent : IEvent {
    CSharp_Int64            GroupUin{};
    CSharp_Int64            OperatorUin{};
    Common::ByteArrayNative Action{};
    Common::ByteArrayNative ActionImgUrl{};
    CSharp_Int64            TargetUin{};
    Common::ByteArrayNative Suffix{};
};
} // namespace Lagrange::Definition::NativeModel::Event