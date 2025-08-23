#pragma once
#include "../Compatibilities.h"

using HCONTEXT = CSharp_Int32;

enum class StatusCode : CSharp_Int32 {
    Success            = 0,
    UnInitialized      = 1,
    AlreadyInitialized = 2,
    AlreadyStarted     = 3,
    InvalidIndex       = 4
};

#include "NativeModel/Common/BotConfig.h"
#include "NativeModel/Common/ByteArrayNative.h"
#include "NativeModel/Common/BotFriendCategory.h"
#include "NativeModel/Common/ByteArrayDictNative.h"
#include "NativeModel/Common/ByteArrayKVPNative.h"
#include "NativeModel/Common/BotGroupNotificationBase.h"
#include "NativeModel/Common/BotGroupJoinNotification.h"
#include "NativeModel/Common/BotGroupInviteNotification.h"
#include "NativeModel/Common/BotKeystore.h"
#include "NativeModel/Message/BotFriend.h"
#include "NativeModel/Message/BotGroup.h"
#include "NativeModel/Message/BotStranger.h"
#include "NativeModel/Message/BotGroupMember.h"
#include "NativeModel/Message/TypedEntity.h"
#include "NativeModel/Message/BotMessage.h"
#include "NativeModel/Event/IEvent.h"
#include "NativeModel/Event/EventArray.h"
#include "NativeModel/Event/BotCaptchaEvent.h"
#include "NativeModel/Event/BotFriendRequestEvent.h"
#include "NativeModel/Event/BotGroupInviteNotificationEvent.h"
#include "NativeModel/Event/BotGroupJoinNotificationEvent.h"
#include "NativeModel/Event/BotGroupMemberDecreaseEvent.h"
#include "NativeModel/Event/BotGroupNudgeEvent.h"
#include "NativeModel/Event/BotGroupReactionEvent.h"
#include "NativeModel/Event/BotLogEvent.h"
#include "NativeModel/Event/BotLoginEvent.h"
#include "NativeModel/Event/BotMessageEvent.h"
#include "NativeModel/Event/BotNewDeviceVerifyEvent.h"
#include "NativeModel/Event/BotOnlineEvent.h"
#include "NativeModel/Event/BotQrCodeEvent.h"
#include "NativeModel/Event/BotQrCodeQueryEvent.h"
#include "NativeModel/Event/BotRefreshKeystoreEvent.h"
#include "NativeModel/Event/BotSMSEvent.h"
#include "NativeModel/Event/ReverseEventCount.h"