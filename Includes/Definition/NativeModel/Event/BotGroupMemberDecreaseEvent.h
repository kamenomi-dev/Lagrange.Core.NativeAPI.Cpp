#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotGroupMemberDecreaseEvent : IEvent {
    CSharp_Int64 GroupUin{NULL};
    CSharp_Int64 UserUin{NULL};
    CSharp_Int64 OperatorUin{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Event