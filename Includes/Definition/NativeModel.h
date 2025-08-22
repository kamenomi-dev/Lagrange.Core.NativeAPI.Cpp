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
#include "NativeModel/Common/BotKeystore.h"
#include "NativeModel/Message/BotGroup.h"
#include "NativeModel/Message/BotMessage.h"
#include "NativeModel/Event/IEvent.h"
#include "NativeModel/Event/EventArray.h"
#include "NativeModel/Event/BotQrCodeEvent.h"
#include "NativeModel/Event/BotMessageEvent.h"
#include "NativeModel/Event/BotRefreshKeystoreEvent.h"
