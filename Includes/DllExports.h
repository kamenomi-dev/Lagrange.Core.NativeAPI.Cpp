#pragma once
#include <string>

// I Hate Windows Header Most.
#ifdef _WIN32
#include <Windows.h>

#define LIBRARY_EXTENSION L".dll"
#define LIBRARY_HANDLE    HMODULE
#endif

#ifdef __linux__
#include <dlfcn.h>

#define LIBRARY_EXTENSION L".so"
#define LIBRARY_HANDLE    void*
#endif

#include "Logger.h"

namespace Lagrange::DllExports {
static LIBRARY_HANDLE handle{};

extern void Initialize(
    std::wstring libraryFile = L"Lagrange.Core.NativeAPI" LIBRARY_EXTENSION
) {
#ifdef _WIN32
    handle = LoadLibraryW(libraryFile.c_str());
    if (!handle) {
        DWORD errorCode = GetLastError();
        Logger::critical(L"Failed to load NativeAPI library: {} (Error code: {})", libraryFile, errorCode);
        exit(EXIT_FAILURE);
        return;
    }

    Logger::debug("NativeAPI loaded successfully.");
    return;
#elif defined(__linux__)
    handle = dlopen(libraryFile.c_str(), RTLD_LAZY);
    if (!handle) {
        Logger::critical("Failed to load NativeAPI library: {} (Error: {})", libraryFile, std::string(dlerror()));
        exit(EXIT_FAILURE);
    }

    Logger::debug("NativeAPI loaded successfully.");
    return;
#else
    Logger::critical("Cannot load library on unknown system.");
    exit(EXIT_FAILURE);
#endif
}

extern void Uninitialize() {
#ifdef _WIN32
    if (handle) {
        FreeLibrary(handle);
    }
#endif

#ifdef __linux__
    if (handle) {
        dlclose(handle);
    }
#endif

    handle = nullptr;
}

template <typename RetType>
extern RetType GetFunctionAddress(
    const char* functionName
) {
    if (!handle) {
        Logger::error("WTF, is it right?");
        abort();
    }

    void* pFn = nullptr;

#ifdef _WIN32
    pFn = GetProcAddress(handle, functionName);
    if (!pFn) {
        Logger::error("Failed to get function address: {} (Error code: {})", functionName, GetLastError());
    }

    return (RetType)pFn;
#elif defined(__linux__)
    pFn = dlsym(handle, functionName);
    if (!pFn) {
        Logger::error("Failed to get function address: {} (Error: {})", functionName, std::string(dlerror()));
    }

    return (RetType)pFn;
#else
    Logger::error("Cannot get function address on unknown system.");
    abort();
#endif
}

// EventEntryPoint.cs

extern auto GetEventCount(
    HCONTEXT hContext
) {
    using FnGetEventCount       = Definition::NativeModel::Event::ReverseEventCount*(__cdecl*)(HCONTEXT);
    static auto fnGetEventCount = GetFunctionAddress<FnGetEventCount>("GetEventCount");

    return fnGetEventCount(hContext);
}

extern auto GetCaptchaEvent(
    HCONTEXT hContext
) {
    using FnGetCaptchaEvent       = Definition::NativeModel::Event::BotCaptchaEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetCaptchaEvent = GetFunctionAddress<FnGetCaptchaEvent>("GetCaptchaEvent");

    return fnGetCaptchaEvent(hContext);
}

extern auto GetBotFriendRequestEvent(
    HCONTEXT hContext
) {
    using FnGetBotFriendRequestEvent       = Definition::NativeModel::Event::BotFriendRequestEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetBotFriendRequestEvent = GetFunctionAddress<FnGetBotFriendRequestEvent>("GetBotFriendRequestEvent");

    return fnGetBotFriendRequestEvent(hContext);
};

extern auto GetGroupInviteNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupInviteNotificationEvent = Definition::NativeModel::Event::BotGroupInviteNotificationEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetGroupInviteNotificationEvent =
        GetFunctionAddress<FnGetGroupInviteNotificationEvent>("GetGroupInviteNotificationEvent");

    return fnGetGroupInviteNotificationEvent(hContext);
}

extern auto GetGroupJoinNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupJoinNotificationEvent = Definition::NativeModel::Event::BotGroupJoinNotificationEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetGroupJoinNotificationEvent =
        GetFunctionAddress<FnGetGroupJoinNotificationEvent>("GetGroupJoinNotificationEvent");

    return fnGetGroupJoinNotificationEvent(hContext);
}

extern auto GetGroupMemberDecreaseEvent(
    HCONTEXT hContext
) {
    using FnGetGroupMemberDecreaseEvent = Definition::NativeModel::Event::BotGroupMemberDecreaseEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetGroupMemberDecreaseEvent =
        GetFunctionAddress<FnGetGroupMemberDecreaseEvent>("GetGroupMemberDecreaseEvent");

    return fnGetGroupMemberDecreaseEvent(hContext);
}

extern auto GetGroupNudgeEvent(
    HCONTEXT hContext
) {
    using FnGetGroupNudgeEvent       = Definition::NativeModel::Event::BotGroupNudgeEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetGroupNudgeEvent = GetFunctionAddress<FnGetGroupNudgeEvent>("GetGroupNudgeEvent");

    return fnGetGroupNudgeEvent(hContext);
}

extern auto GetBotGroupReactionEvent(
    HCONTEXT hContext
) {
    using FnGetBotGroupReactionEvent       = Definition::NativeModel::Event::BotGroupReactionEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetBotGroupReactionEvent = GetFunctionAddress<FnGetBotGroupReactionEvent>("GetBotGroupReactionEvent");

    return fnGetBotGroupReactionEvent(hContext);
};

extern auto GetLoginEvent(
    HCONTEXT hContext
) {
    using FnGetLoginEvent       = Definition::NativeModel::Event::BotLoginEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetLoginEvent = GetFunctionAddress<FnGetLoginEvent>("GetLoginEvent");

    return fnGetLoginEvent(hContext);
}

extern auto GetBotLogEvent(
    HCONTEXT hContext
) {
    using FnGetBotLogEvent       = Definition::NativeModel::Event::BotLogEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetBotLogEvent = GetFunctionAddress<FnGetBotLogEvent>("GetBotLogEvent");

    return fnGetBotLogEvent(hContext);
}

extern auto GetMessageEvent(
    HCONTEXT hContext
) {
    using FnGetMessageEvent       = Definition::NativeModel::Event::BotMessageEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetMessageEvent = GetFunctionAddress<FnGetMessageEvent>("GetMessageEvent");

    return fnGetMessageEvent(hContext);
}

extern auto GetNewDeviceVerifyEvent(
    HCONTEXT hContext
) {
    using FnGetNewDeviceVerifyEvent       = Definition::NativeModel::Event::BotNewDeviceVerifyEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetNewDeviceVerifyEvent = GetFunctionAddress<FnGetNewDeviceVerifyEvent>("GetNewDeviceVerifyEvent");

    return fnGetNewDeviceVerifyEvent(hContext);
}

extern auto GetOnlineEvent(
    HCONTEXT hContext
) {
    using FnGetOnlineEvent       = Definition::NativeModel::Event::BotOnlineEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetOnlineEvent = GetFunctionAddress<FnGetOnlineEvent>("GetOnlineEvent");

    return fnGetOnlineEvent(hContext);
}

extern auto GetQrCodeEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeEvent       = Definition::NativeModel::Event::BotQrCodeEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetQrCodeEvent = GetFunctionAddress<FnGetQrCodeEvent>("GetQrCodeEvent");

    return fnGetQrCodeEvent(hContext);
}

extern auto GetQrCodeQueryEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeQueryEvent       = Definition::NativeModel::Event::BotQrCodeQueryEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetQrCodeQueryEvent = GetFunctionAddress<FnGetQrCodeQueryEvent>("GetQrCodeQueryEvent");

    return fnGetQrCodeQueryEvent(hContext);
}

extern auto GetRefreshKeystoreEvent(
    HCONTEXT hContext
) {
    using FnGetRefreshKeystoreEvent       = Definition::NativeModel::Event::BotRefreshKeystoreEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetRefreshKeystoreEvent = GetFunctionAddress<FnGetRefreshKeystoreEvent>("GetRefreshKeystoreEvent");

    return fnGetRefreshKeystoreEvent(hContext);
}

extern auto GetSMSEvent(
    HCONTEXT hContext
) {
    using FnGetSMSEvent       = Definition::NativeModel::Event::BotSMSEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetSMSEvent = GetFunctionAddress<FnGetSMSEvent>("GetSMSEvent");

    return fnGetSMSEvent(hContext);
}

// SendMessageEntryPoint.cs

extern HCONTEXT CreateMessageBuilder(
    HCONTEXT hContext
) {
    using FnCreateMessageBuilder       = HCONTEXT(__cdecl*)(HCONTEXT);
    static auto fnCreateMessageBuilder = GetFunctionAddress<FnCreateMessageBuilder>("CreateMessageBuilder");

    return fnCreateMessageBuilder(hContext);
}

extern void AddText(
    HCONTEXT hContext, HCONTEXT hInstanceContext, Definition::NativeModel::Common::ByteArrayNative text
) {
    using FnAddText       = void(__cdecl*)(HCONTEXT, HCONTEXT, Definition::NativeModel::Common::ByteArrayNative);
    static auto fnAddText = GetFunctionAddress<FnAddText>("AddText");

    return fnAddText(hContext, hInstanceContext, text);
}

extern void AddImage(
    HCONTEXT                                         hContext,
    HCONTEXT                                         hInstanceContext,
    Definition::NativeModel::Common::ByteArrayNative imageData,
    Definition::NativeModel::Common::ByteArrayNative summary,
    CSharp_Int32                                     subType
) {
    using FnAddImage = void(__cdecl*)(
        HCONTEXT,
        HCONTEXT,
        Definition::NativeModel::Common::ByteArrayNative,
        Definition::NativeModel::Common::ByteArrayNative,
        CSharp_Int32
    );
    static auto fnAddImage = GetFunctionAddress<FnAddImage>("AddImage");

    return fnAddImage(hContext, hInstanceContext, imageData, summary, subType);
}

extern void AddLocalImage(
    HCONTEXT                                         hContext,
    HCONTEXT                                         hInstanceContext,
    Definition::NativeModel::Common::ByteArrayNative imagePath,
    Definition::NativeModel::Common::ByteArrayNative summary,
    CSharp_Int32                                     subType
) {
    using FnAddLocalImage = void(__cdecl*)(
        HCONTEXT,
        HCONTEXT,
        Definition::NativeModel::Common::ByteArrayNative,
        Definition::NativeModel::Common::ByteArrayNative,
        CSharp_Int32
    );
    static auto fnAddLocalImage = GetFunctionAddress<FnAddLocalImage>("AddLocalImage");

    return fnAddLocalImage(hContext, hInstanceContext, imagePath, summary, subType);
}

extern void AddRecord(
    HCONTEXT hContext, HCONTEXT hInstanceContext, Definition::NativeModel::Common::ByteArrayNative recordData
) {
    using FnAddRecord       = void(__cdecl*)(HCONTEXT, HCONTEXT, Definition::NativeModel::Common::ByteArrayNative);
    static auto fnAddRecord = GetFunctionAddress<FnAddRecord>("AddRecord");

    return fnAddRecord(hContext, hInstanceContext, recordData);
}

extern void AddLocalRecord(
    HCONTEXT hContext, HCONTEXT hInstanceContext, Definition::NativeModel::Common::ByteArrayNative recordPath
) {
    using FnAddLocalRecord       = void(__cdecl*)(HCONTEXT, HCONTEXT, Definition::NativeModel::Common::ByteArrayNative);
    static auto fnAddLocalRecord = GetFunctionAddress<FnAddLocalRecord>("AddLocalRecord");

    return fnAddLocalRecord(hContext, hInstanceContext, recordPath);
}

extern void AddVideo(
    HCONTEXT                                         hContext,
    HCONTEXT                                         hInstanceContext,
    Definition::NativeModel::Common::ByteArrayNative videoData,
    Definition::NativeModel::Common::ByteArrayNative thumbnailData
) {
    using FnAddVideo = void(__cdecl*)(
        HCONTEXT,
        HCONTEXT,
        Definition::NativeModel::Common::ByteArrayNative,
        Definition::NativeModel::Common::ByteArrayNative
    );
    static auto fnAddVideo = GetFunctionAddress<FnAddVideo>("AddVideo");

    return fnAddVideo(hContext, hInstanceContext, videoData, thumbnailData);
}

extern void AddLocalVideo(
    HCONTEXT                                         hContext,
    HCONTEXT                                         hInstanceContext,
    Definition::NativeModel::Common::ByteArrayNative videoPath,
    Definition::NativeModel::Common::ByteArrayNative thumbnailPath
) {
    using FnAddLocalVideo = void(__cdecl*)(
        HCONTEXT,
        HCONTEXT,
        Definition::NativeModel::Common::ByteArrayNative,
        Definition::NativeModel::Common::ByteArrayNative
    );
    static auto fnAddLocalVideo = GetFunctionAddress<FnAddLocalVideo>("AddLocalVideo");

    return fnAddLocalVideo(hContext, hInstanceContext, videoPath, thumbnailPath);
}

extern auto SendFriendMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 userId
) {
    using FnSendFriendMessage =
        Definition::NativeModel::Message::BotMessage*(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendFriendMessage = GetFunctionAddress<FnSendFriendMessage>("SendFriendMessage");

    return fnSendFriendMessage(hContext, hInstanceContext, userId);
}

extern auto SendGroupMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 groupId
) {
    using FnSendGroupMessage =
        Definition::NativeModel::Message::BotMessage*(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendGroupMessage = GetFunctionAddress<FnSendGroupMessage>("SendGroupMessage");

    return fnSendGroupMessage(hContext, hInstanceContext, groupId);
}

// Program.cs

extern HCONTEXT Initialize(
    Definition::NativeModel::Common::BotConfig* config, Definition::NativeModel::Common::BotKeystore* keystore
) {
    using FnInitialize       = HCONTEXT(__cdecl*)(void*, void*);
    static auto fnInitialize = GetFunctionAddress<FnInitialize>("Initialize");

    return fnInitialize(config, keystore);
}

extern StatusCode Start(
    HCONTEXT hContext
) {
    using FnStart       = StatusCode(__cdecl*)(HCONTEXT);
    static auto fnStart = GetFunctionAddress<FnStart>("Start");

    return fnStart(hContext);
}

extern StatusCode Stop(
    HCONTEXT hContext
) {
    using FnStop       = StatusCode(__cdecl*)(HCONTEXT);
    static auto fnStop = GetFunctionAddress<FnStop>("Stop");

    return fnStop(hContext);
}

extern void FreeMemory(
    void* ptr
) {
    using FnFreeMemory       = void(__cdecl*)(void*);
    static auto fnFreeMemory = GetFunctionAddress<FnFreeMemory>("FreeMemory");

    return fnFreeMemory(ptr);
}
} // namespace Lagrange::DllExports

#undef LIBRARY_EXTENSION
#undef LIBRARY_HANDLE
