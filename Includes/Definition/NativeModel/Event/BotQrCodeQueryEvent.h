#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotQrCodeQueryEvent : IEvent {
    enum TransEmpState : CSharp_Byte {
        Confirmed         = 0,
        CodeExpired       = 17,
        WaitingForScan    = 48,
        WaitingForConfirm = 53,
        Canceled          = 54,
        Invalid           = 144
    };

    TransEmpState State{TransEmpState::Confirmed};
};
} // namespace Lagrange::Definition::NativeModel::Event