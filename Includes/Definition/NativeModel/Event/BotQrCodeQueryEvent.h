#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct BotQrCodeQueryEvent : IEvent {
    enum class TransEmpState : CSharp_Byte {
        Confirmed         = 0,
        CodeExpired       = 17,
        WaitingForScan    = 48,
        WaitingForConfirm = 53,
        Canceled          = 54,
        Invalid           = 144
    };

    TransEmpState State{TransEmpState::Confirmed};

    const char* TransEmpStateToEnumTag() {
        switch (State) {
        case TransEmpState::Confirmed:
            return "Confirmed";
        case TransEmpState::CodeExpired:
            return "CodeExpired";
        case TransEmpState::WaitingForScan:
            return "WaitingForScan";
        case TransEmpState::WaitingForConfirm:
            return "WaitingForConfirm";
        case TransEmpState::Canceled:
            return "Canceled";
        case TransEmpState::Invalid:
            return "Invalid";
        default:
            return "Unknown";
        }
    }

    // As Simplified Chinese Detail.
    const char* TransEmpStateToDetail() {
        switch (State) {
        case TransEmpState::Confirmed:
            return "已确认";
        case TransEmpState::CodeExpired:
            return "二维码过期";
        case TransEmpState::WaitingForScan:
            return "等待扫描";
        case TransEmpState::WaitingForConfirm:
            return "等待确认";
        case TransEmpState::Canceled:
            return "已取消";
        case TransEmpState::Invalid:
            return "无效";
        default:
            return "未知";
        }
    }
};
} // namespace Lagrange::Definition::NativeModel::Event