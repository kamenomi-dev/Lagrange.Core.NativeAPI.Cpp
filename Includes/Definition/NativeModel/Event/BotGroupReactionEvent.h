#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotGroupReactionEvent : IEvent {
    CSharp_Int64            TargetGroupUin{NULL};
    CSharp_UInt64           TargetSequence{NULL};
    CSharp_Int64            OperatorUin{NULL};
    CSharp_Boolean          IsAdd{false};
    Common::ByteArrayNative Code{};
    CSharp_UInt64           CurrentCount{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Event