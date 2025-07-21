#pragma once
#pragma region DllLoad

#include "logger.cpp"
#include "native_model.h"

#include <Windows.h>
#include <string>
#include <exception>

namespace LagrangeCore {
enum class StatusCode {
    Success = 0,
    UnInitialized,
    AlreadyInitialized,
    AlreadyStarted,
    InvalidIndex
};

using ContextIndex = int;

class DllExportsImpl {
  public:
    explicit DllExportsImpl(const std::wstring& dllPath = L"Lagrange.Core.NativeAPI.dll");

    void       LoadNativeAPI();
    void       UnloadLibrary();
    const bool IsLoaded() { return _hModule != nullptr; }

    // No Copy.
    DllExportsImpl(const DllExportsImpl&)            = delete;
    DllExportsImpl& operator=(const DllExportsImpl&) = delete;

    ~DllExportsImpl();

  public: // ------------------------- APIs ----------------------
    using FnInitialize =
        ContextIndex(__cdecl*)(_In_ NativeModel::Common::BotConfig*, _In_ NativeModel::Common::BotKeystore*);
    using FnStart         = StatusCode(__cdecl*)(_In_ ContextIndex);
    using FnStop          = StatusCode(__cdecl*)(_In_ ContextIndex);
    using FnFreeMemory    = void(__cdecl*)(_In_ INT_PTR);
    using FnGetEventCount = INT_PTR(__cdecl*)(_In_ ContextIndex);

    // Events
    using FnGetLoginEvent           = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetOnlineEvent          = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetMessageEvent         = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetBotLogEvent          = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetSMSEvent             = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetQrCodeEvent          = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetQrCodeQueryEvent     = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetRefreshKeytoreEvent  = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnGetNewDeviceVerifyEvent = INT_PTR(__cdecl*)(_In_ ContextIndex);

    // Actions
    using FnSendGroupMessage  = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnSendFriendMessage = INT_PTR(__cdecl*)(_In_ ContextIndex);


    template <typename FuncType>
    void LoadMethod(
        FuncType& funcPtr, const char* funcName
    ) {
        funcPtr = reinterpret_cast<FuncType>(GetProcAddress(_hModule, funcName));
        if (!funcPtr) {
            spdlog::error("Unknown interface. Is that using an incompatible library? ");
            return;
        }
    }

    ContextIndex Initialize(_In_ NativeModel::Common::BotConfig*, _In_ NativeModel::Common::BotKeystore* = nullptr);
    StatusCode   Start(_In_ ContextIndex);
    StatusCode   Stop(_In_ ContextIndex);
    void         FreeMemory(_In_ INT_PTR);
    INT_PTR      GetEventCount(_In_ ContextIndex);
    INT_PTR      GetQrCodeEvent(_In_ ContextIndex);
    INT_PTR      GetBotLogEvent(_In_ ContextIndex);

    HMODULE      _hModule = nullptr;
    std::wstring _dllPath = L"Lagrange.Core.NativeAPI.dll";

    FnInitialize     _InitializeFunc     = nullptr;
    FnStart          _StartFunc          = nullptr;
    FnStop           _StopFunc           = nullptr;
    FnFreeMemory     _FreeMemoryFunc     = nullptr;
    FnGetEventCount  _GetEventCountFunc  = nullptr;
    FnGetQrCodeEvent _GetQrCodeEventFunc = nullptr;
    FnGetBotLogEvent _GetBotLogEventFunc = nullptr;
};
} // namespace LagrangeCore

#pragma endregion DllLoad