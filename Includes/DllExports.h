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

extern CSharp_IntPtr GetEventCount(
    HCONTEXT hContext
) {
    using FnGetEventCount       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetEventCount = GetFunctionAddress<FnGetEventCount>("GetEventCount");

    return fnGetEventCount(hContext);
}

extern CSharp_IntPtr GetCaptchaEvent(
    HCONTEXT hContext
) {
    using FnGetCaptchaEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetCaptchaEvent = GetFunctionAddress<FnGetCaptchaEvent>("GetCaptchaEvent");

    return fnGetCaptchaEvent(hContext);
}

extern CSharp_IntPtr GetBotFriendRequestEvent(
    HCONTEXT hContext
) {
    using FnGetBotFriendRequestEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetBotFriendRequestEvent = GetFunctionAddress<FnGetBotFriendRequestEvent>("GetBotFriendRequestEvent");

    return fnGetBotFriendRequestEvent(hContext);
};

extern CSharp_IntPtr GetGroupInviteNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupInviteNotificationEvent = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetGroupInviteNotificationEvent =
        GetFunctionAddress<FnGetGroupInviteNotificationEvent>("GetGroupInviteNotificationEvent");

    return fnGetGroupInviteNotificationEvent(hContext);
}

extern CSharp_IntPtr GetGroupJoinNotificationEvent(
    HCONTEXT hContext
) {
    using FnGetGroupJoinNotificationEvent = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetGroupJoinNotificationEvent =
        GetFunctionAddress<FnGetGroupJoinNotificationEvent>("GetGroupJoinNotificationEvent");

    return fnGetGroupJoinNotificationEvent(hContext);
}

extern CSharp_IntPtr GetGroupMemberDecreaseEvent(
    HCONTEXT hContext
) {
    using FnGetGroupMemberDecreaseEvent = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetGroupMemberDecreaseEvent =
        GetFunctionAddress<FnGetGroupMemberDecreaseEvent>("GetGroupMemberDecreaseEvent");

    return fnGetGroupMemberDecreaseEvent(hContext);
}

extern CSharp_IntPtr GetGroupNudgeEvent(
    HCONTEXT hContext
) {
    using FnGetGroupNudgeEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetGroupNudgeEvent = GetFunctionAddress<FnGetGroupNudgeEvent>("GetGroupNudgeEvent");

    return fnGetGroupNudgeEvent(hContext);
}

extern CSharp_IntPtr GetBotGroupReactionEvent(
    HCONTEXT hContext
) {
    using FnGetBotGroupReactionEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetBotGroupReactionEvent = GetFunctionAddress<FnGetBotGroupReactionEvent>("GetBotGroupReactionEvent");

    return fnGetBotGroupReactionEvent(hContext);
};

extern CSharp_IntPtr GetLoginEvent(
    HCONTEXT hContext
) {
    using FnGetLoginEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetLoginEvent = GetFunctionAddress<FnGetLoginEvent>("GetLoginEvent");

    return fnGetLoginEvent(hContext);
}

extern CSharp_IntPtr GetBotLogEvent(
    HCONTEXT hContext
) {
    using FnGetBotLogEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetBotLogEvent = GetFunctionAddress<FnGetBotLogEvent>("GetBotLogEvent");

    return fnGetBotLogEvent(hContext);
}

extern CSharp_IntPtr GetMessageEvent(
    HCONTEXT hContext
) {
    using FnGetMessageEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetMessageEvent = GetFunctionAddress<FnGetMessageEvent>("GetMessageEvent");

    return fnGetMessageEvent(hContext);
}

extern CSharp_IntPtr GetNewDeviceVerifyEvent(
    HCONTEXT hContext
) {
    using FnGetNewDeviceVerifyEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetNewDeviceVerifyEvent = GetFunctionAddress<FnGetNewDeviceVerifyEvent>("GetNewDeviceVerifyEvent");

    return fnGetNewDeviceVerifyEvent(hContext);
}

extern CSharp_IntPtr GetOnlineEvent(
    HCONTEXT hContext
) {
    using FnGetOnlineEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetOnlineEvent = GetFunctionAddress<FnGetOnlineEvent>("GetOnlineEvent");

    return fnGetOnlineEvent(hContext);
}

extern CSharp_IntPtr GetQrCodeEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetQrCodeEvent = GetFunctionAddress<FnGetQrCodeEvent>("GetQrCodeEvent");

    return fnGetQrCodeEvent(hContext);
}

extern CSharp_IntPtr GetQrCodeQueryEvent(
    HCONTEXT hContext
) {
    using FnGetQrCodeQueryEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetQrCodeQueryEvent = GetFunctionAddress<FnGetQrCodeQueryEvent>("GetQrCodeQueryEvent");

    return fnGetQrCodeQueryEvent(hContext);
}

extern CSharp_IntPtr GetRefreshKeystoreEvent(
    HCONTEXT hContext
) {
    using FnGetRefreshKeystoreEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
    static auto fnGetRefreshKeystoreEvent = GetFunctionAddress<FnGetRefreshKeystoreEvent>("GetRefreshKeystoreEvent");

    return fnGetRefreshKeystoreEvent(hContext);
}

extern CSharp_IntPtr GetSMSEvent(
    HCONTEXT hContext
) {
    using FnGetSMSEvent       = CSharp_IntPtr(__cdecl*)(HCONTEXT);
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
    CSharp_Int32                                       subType
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
    CSharp_Int32                                       subType
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

extern CSharp_IntPtr SendFriendMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 userId
) {
    using FnSendFriendMessage       = CSharp_IntPtr(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendFriendMessage = GetFunctionAddress<FnSendFriendMessage>("SendFriendMessage");

    return fnSendFriendMessage(hContext, hInstanceContext, userId);
}

extern CSharp_IntPtr SendGroupMessage(
    HCONTEXT hContext, HCONTEXT hInstanceContext, CSharp_Int64 groupId
) {
    using FnSendGroupMessage       = CSharp_IntPtr(__cdecl*)(HCONTEXT, HCONTEXT, CSharp_Int64);
    static auto fnSendGroupMessage = GetFunctionAddress<FnSendGroupMessage>("SendGroupMessage");

    return fnSendGroupMessage(hContext, hInstanceContext, groupId);
}

// Program.cs

extern HCONTEXT Initialize(const Definition::NativeModel::Common::BotConfig* config, const Definition::NativeModel::Common::BotKeystore* keystore) {
    using FnInitialize       = HCONTEXT(__cdecl*)(CSharp_IntPtr, CSharp_IntPtr);
    static auto fnInitialize = GetFunctionAddress<FnInitialize>("Initialize");

    return fnInitialize((CSharp_IntPtr)config, (CSharp_IntPtr)keystore);
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
    CSharp_IntPtr ptr
) {
    using FnFreeMemory       = void(__cdecl*)(CSharp_IntPtr);
    static auto fnFreeMemory = GetFunctionAddress<FnFreeMemory>("FreeMemory");

    return fnFreeMemory(ptr);
}
} // namespace Lagrange::DllExports

#undef LIBRARY_EXTENSION
#undef LIBRARY_HANDLE
