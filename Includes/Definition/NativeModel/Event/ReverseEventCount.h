#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct ReverseEventCount : IEvent {
    CSharp_Int32 BotCaptchaEventCount = 0;
    CSharp_Int32 BotFriendRequestEventCount = 0;
    CSharp_Int32 BotGroupInviteNotificationEventCount = 0;
    CSharp_Int32 BotGroupInviteSelfEventCount = 0;
    CSharp_Int32 BotGroupJoinNotificationEventCount = 0;
    CSharp_Int32 BotGroupMemberDecreaseEventCount = 0;
    CSharp_Int32 BotGroupNudgeEventCount = 0;
    CSharp_Int32 BotGroupReactionEventCount = 0;
    CSharp_Int32 BotLoginEventCount = 0;
    CSharp_Int32 BotLogEventCount = 0;
    CSharp_Int32 BotMessageEventCount = 0;
    CSharp_Int32 BotNewDeviceVerifyEventCount = 0;
    CSharp_Int32 BotOnlineEventCount = 0;
    CSharp_Int32 BotQrCodeEventCount = 0;
    CSharp_Int32 BotQrCodeQueryEventCount = 0;
    CSharp_Int32 BotRefreshKeystoreEventCount = 0;
    CSharp_Int32 BotSMSEventCount = 0;
};
}