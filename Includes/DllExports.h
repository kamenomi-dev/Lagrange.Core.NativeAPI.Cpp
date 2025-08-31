#pragma once
#include <string>

#define NATIVE_MODEL Lagrange::Definition::NativeModel::

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
    using FnGetEventCount                                = NATIVE_MODEL Event::ReverseEventCount*(__cdecl*)(HCONTEXT);
    static auto                          fnGetEventCount = GetFunctionAddress<FnGetEventCount>("GetEventCount");

    return fnGetEventCount(hContext);
}

extern auto GetCaptchaEvent(
    HCONTEXT hContext
) {
    using FnGetCaptchaEvent                                  = NATIVE_MODEL Event::BotCaptchaEvent*(__cdecl*)(HCONTEXT);
    static auto                            fnGetCaptchaEvent = GetFunctionAddress<FnGetCaptchaEvent>("GetCaptchaEvent");

    return fnGetCaptchaEvent(hContext);
}

extern auto GetBotFriendRequestEvent(
    HCONTEXT hContext
) {
    using FnGetBotFriendRequestEvent       = NATIVE_MODEL Event::BotFriendRequestEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetBotFriendRequestEvent = GetFunctionAddress<FnGetBotFriendRequestEvent>("GetBotFriendRequestEvent");

    return fnGetBotFriendRequestEvent(hContext);
};

extern auto GetGroupInviteNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupInviteNotificationEvent = NATIVE_MODEL Event::BotGroupInviteNotificationEvent*(__cdecl*)(HCONTEXT);
    static auto                                            fnGetGroupInviteNotificationEvent =
        GetFunctionAddress<FnGetGroupInviteNotificationEvent>("GetGroupInviteNotificationEvent");

    return fnGetGroupInviteNotificationEvent(hContext);
}

extern auto GetGroupJoinNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupJoinNotificationEvent = NATIVE_MODEL Event::BotGroupJoinNotificationEvent*(__cdecl*)(HCONTEXT);
    static auto                                          fnGetGroupJoinNotificationEvent =
        GetFunctionAddress<FnGetGroupJoinNotificationEvent>("GetGroupJoinNotificationEvent");

    return fnGetGroupJoinNotificationEvent(hContext);
}

extern auto GetGroupMemberDecreaseEvent(
    HCONTEXT hContext
) {
    using FnGetGroupMemberDecreaseEvent = NATIVE_MODEL Event::BotGroupMemberDecreaseEvent*(__cdecl*)(HCONTEXT);
    static auto                                        fnGetGroupMemberDecreaseEvent =
        GetFunctionAddress<FnGetGroupMemberDecreaseEvent>("GetGroupMemberDecreaseEvent");

    return fnGetGroupMemberDecreaseEvent(hContext);
}

extern auto GetGroupNudgeEvent(
    HCONTEXT hContext
) {
    using FnGetGroupNudgeEvent       = NATIVE_MODEL Event::BotGroupNudgeEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetGroupNudgeEvent = GetFunctionAddress<FnGetGroupNudgeEvent>("GetGroupNudgeEvent");

    return fnGetGroupNudgeEvent(hContext);
}

extern auto GetBotGroupReactionEvent(
    HCONTEXT hContext
) {
    using FnGetBotGroupReactionEvent       = NATIVE_MODEL Event::BotGroupReactionEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetBotGroupReactionEvent = GetFunctionAddress<FnGetBotGroupReactionEvent>("GetBotGroupReactionEvent");

    return fnGetBotGroupReactionEvent(hContext);
};

extern auto GetLoginEvent(
    HCONTEXT hContext
) {
    using FnGetLoginEvent                                = NATIVE_MODEL Event::BotLoginEvent*(__cdecl*)(HCONTEXT);
    static auto                          fnGetLoginEvent = GetFunctionAddress<FnGetLoginEvent>("GetLoginEvent");

    return fnGetLoginEvent(hContext);
}

extern auto GetBotLogEvent(
    HCONTEXT hContext
) {
    using FnGetBotLogEvent                                 = NATIVE_MODEL Event::BotLogEvent*(__cdecl*)(HCONTEXT);
    static auto                           fnGetBotLogEvent = GetFunctionAddress<FnGetBotLogEvent>("GetBotLogEvent");

    return fnGetBotLogEvent(hContext);
}

extern auto GetMessageEvent(
    HCONTEXT hContext
) {
    using FnGetMessageEvent                                  = NATIVE_MODEL Event::BotMessageEvent*(__cdecl*)(HCONTEXT);
    static auto                            fnGetMessageEvent = GetFunctionAddress<FnGetMessageEvent>("GetMessageEvent");

    return fnGetMessageEvent(hContext);
}

extern auto GetNewDeviceVerifyEvent(
    HCONTEXT hContext
) {
    using FnGetNewDeviceVerifyEvent       = NATIVE_MODEL Event::BotNewDeviceVerifyEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetNewDeviceVerifyEvent = GetFunctionAddress<FnGetNewDeviceVerifyEvent>("GetNewDeviceVerifyEvent");

    return fnGetNewDeviceVerifyEvent(hContext);
}

extern auto GetOnlineEvent(
    HCONTEXT hContext
) {
    using FnGetOnlineEvent                                 = NATIVE_MODEL Event::BotOnlineEvent*(__cdecl*)(HCONTEXT);
    static auto                           fnGetOnlineEvent = GetFunctionAddress<FnGetOnlineEvent>("GetOnlineEvent");

    return fnGetOnlineEvent(hContext);
}

extern auto GetQrCodeEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeEvent                                 = NATIVE_MODEL Event::BotQrCodeEvent*(__cdecl*)(HCONTEXT);
    static auto                           fnGetQrCodeEvent = GetFunctionAddress<FnGetQrCodeEvent>("GetQrCodeEvent");

    return fnGetQrCodeEvent(hContext);
}

extern auto GetQrCodeQueryEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeQueryEvent       = NATIVE_MODEL Event::BotQrCodeQueryEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetQrCodeQueryEvent = GetFunctionAddress<FnGetQrCodeQueryEvent>("GetQrCodeQueryEvent");

    return fnGetQrCodeQueryEvent(hContext);
}

extern auto GetRefreshKeystoreEvent(
    HCONTEXT hContext
) {
    using FnGetRefreshKeystoreEvent       = NATIVE_MODEL Event::BotRefreshKeystoreEvent*(__cdecl*)(HCONTEXT);
    static auto fnGetRefreshKeystoreEvent = GetFunctionAddress<FnGetRefreshKeystoreEvent>("GetRefreshKeystoreEvent");

    return fnGetRefreshKeystoreEvent(hContext);
}

extern auto GetSMSEvent(
    HCONTEXT hContext
) {
    using FnGetSMSEvent                              = NATIVE_MODEL Event::BotSMSEvent*(__cdecl*)(HCONTEXT);
    static auto                        fnGetSMSEvent = GetFunctionAddress<FnGetSMSEvent>("GetSMSEvent");

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
    HCONTEXT hContext, HCONTEXT hInstanceContext, std::u8string text
) {
    using FnAddText       = void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddText = GetFunctionAddress<FnAddText>("AddText");

    return fnAddText(hContext, hInstanceContext, text);
}

extern void AddImage(
    HCONTEXT     hContext,
    HCONTEXT     hInstanceContext,
    NATIVE_MODEL Common::ByteArrayNative imageData,
    std::u8string                        summary = u8"",
    CSharp_Int32                         subType = 0
) {
    using FnAddImage = void(__cdecl*)(
        HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative, NATIVE_MODEL Common::ByteArrayNative, CSharp_Int32
    );
    static auto fnAddImage = GetFunctionAddress<FnAddImage>("AddImage");

    return fnAddImage(
        hContext,
        hInstanceContext,
        imageData,
        summary.empty() ? NATIVE_MODEL Common::ByteArrayNative{0, nullptr}
                        : NATIVE_MODEL Common::ByteArrayNative{summary},
        subType
    );
}

extern void AddLocalImage(
    HCONTEXT      hContext,
    HCONTEXT      hInstanceContext,
    std::u8string imagePath,
    std::u8string summary = u8"",
    CSharp_Int32  subType = 0
) {
    using FnAddLocalImage = void(__cdecl*)(
        HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative, NATIVE_MODEL Common::ByteArrayNative, CSharp_Int32
    );
    static auto fnAddLocalImage = GetFunctionAddress<FnAddLocalImage>("AddLocalImage");

    return fnAddLocalImage(
        hContext,
        hInstanceContext,
        imagePath,
        summary.empty() ? NATIVE_MODEL Common::ByteArrayNative{0, nullptr}
                        : NATIVE_MODEL Common::ByteArrayNative{summary},
        subType
    );
}

extern void AddMention(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 uin, NATIVE_MODEL Common::ByteArrayNative display = {}
) {
    using FnAddMention       = void(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddMention = GetFunctionAddress<FnAddMention>("AddMention");

    return fnAddMention(hContext, hInstanceContext, uin, display);
}

extern void AddMultiMsg(
    HCONTEXT hContext, HCONTEXT hInstanceContext, std::u8string resId
) {
    using FnAddMultiMsg       = void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddMultiMsg = GetFunctionAddress<FnAddMultiMsg>("AddMultiMsg");

    return fnAddMultiMsg(hContext, hInstanceContext, resId);
}

extern void AddRecord(
    HCONTEXT hContext, HCONTEXT hInstanceContext, std::u8string recordData
) {
    using FnAddRecord       = void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddRecord = GetFunctionAddress<FnAddRecord>("AddRecord");

    return fnAddRecord(hContext, hInstanceContext, recordData);
}

extern void AddLocalRecord(
    HCONTEXT hContext, HCONTEXT hInstanceContext, std::u8string recordPath
) {
    using FnAddLocalRecord       = void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddLocalRecord = GetFunctionAddress<FnAddLocalRecord>("AddLocalRecord");

    return fnAddLocalRecord(hContext, hInstanceContext, recordPath);
}

extern void AddVideo(
    HCONTEXT     hContext,
    HCONTEXT     hInstanceContext,
    NATIVE_MODEL Common::ByteArrayNative videoData,
    NATIVE_MODEL Common::ByteArrayNative thumbnailData = {}
) {
    using FnAddVideo =
        void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddVideo = GetFunctionAddress<FnAddVideo>("AddVideo");

    return fnAddVideo(hContext, hInstanceContext, videoData, thumbnailData);
}

extern void AddLocalVideo(
    HCONTEXT hContext, HCONTEXT hInstanceContext, std::u8string videoPath, std::u8string thumbnailPath = u8""
) {
    using FnAddLocalVideo =
        void(__cdecl*)(HCONTEXT, HCONTEXT, NATIVE_MODEL Common::ByteArrayNative, NATIVE_MODEL Common::ByteArrayNative);
    static auto fnAddLocalVideo = GetFunctionAddress<FnAddLocalVideo>("AddLocalVideo");

    return fnAddLocalVideo(
        hContext,
        hInstanceContext,
        videoPath,
        thumbnailPath.empty() ? NATIVE_MODEL Common::ByteArrayNative{0, nullptr}
                              : NATIVE_MODEL Common::ByteArrayNative{thumbnailPath}
    );
}

extern auto SendFriendMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 userId
) {
    using FnSendFriendMessage       = NATIVE_MODEL Message::BotMessage*(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendFriendMessage = GetFunctionAddress<FnSendFriendMessage>("SendFriendMessage");

    return fnSendFriendMessage(hContext, hInstanceContext, userId);
}

extern auto SendGroupMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 groupId
) {
    using FnSendGroupMessage       = NATIVE_MODEL Message::BotMessage*(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendGroupMessage = GetFunctionAddress<FnSendGroupMessage>("SendGroupMessage");

    return fnSendGroupMessage(hContext, hInstanceContext, groupId);
}

// Program.cs

extern HCONTEXT Initialize(
    NATIVE_MODEL Common::BotConfig* config, NATIVE_MODEL Common::BotKeystore* keystore
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
