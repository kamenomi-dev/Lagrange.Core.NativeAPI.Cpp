#pragma once

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_NATIVE_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_NATIVE_H_
#pragma region DllLoad

#include "common.h"

#include <Windows.h>
#include <string>
#include <exception>

namespace LagrangeCore {

class DllExportsImpl {
  public:
    explicit DllExportsImpl(const std::wstring& dllPath = L"Lagrange.Core.NativeAPI.dll");

    bool IsLoaded() const { return _hModule != nullptr; }

    // No Copy.
    DllExportsImpl(const DllExportsImpl&)            = delete;
    DllExportsImpl& operator=(const DllExportsImpl&) = delete;
    // DllExportsImpl(DllExportsImpl&& other) noexcept;
    // DllExportsImpl& operator=(DllExportsImpl&& other) noexcept;

    ~DllExportsImpl();

  public: // ------------------------- APIs ----------------------
    using FnInitialize = ContextIndex(__cdecl*)(_In_ BotConfigStruct*, _In_ BotKeystoreStruct*);
    using FnStart = StatusCode(__cdecl*)(ContextIndex);
    using FnStop = StatusCode(__cdecl*)(ContextIndex);
    using FnFreeMemory = void(__cdecl*)(INT_PTR);
    using FnGetEventCount = INT_PTR(__cdecl*)(ContextIndex);

    // Events
    using FnGetLoginEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetOnlineEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetMessageEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetBotLogEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetSMSEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetQrCodeEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetQrCodeQueryEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetRefreshKeytoreEvent = INT_PTR(__cdecl*)(ContextIndex);
    using FnGetNewDeviceVerifyEvent = INT_PTR(__cdecl*)(ContextIndex);

    // Actions
    using FnSengGroupMessage = INT_PTR(__cdecl*)(ContextIndex);
    using FnSendFriendMessage = INT_PTR(__cdecl*)(ContextIndex);


    template <typename FuncType>
    void LoadMethod(
        FuncType& funcPtr, const char* funcName
    ) {
        funcPtr = reinterpret_cast<FuncType>(GetProcAddress(_hModule, funcName));
        if (!funcPtr) {
            DWORD error = GetLastError();
            abort();
        }
    }

    ContextIndex Initialize(_In_ BotConfigStruct*, _In_ BotKeystoreStruct* = nullptr);
    StatusCode   Start(_In_ ContextIndex);
    StatusCode   Stop(_In_ ContextIndex);
    void         FreeMemory(_In_ INT_PTR);
    INT_PTR      GetEventCount(_In_ ContextIndex);
    INT_PTR      GetQrCodeEvent(_In_ ContextIndex);
    INT_PTR      GetBotLogEvent(_In_ ContextIndex);

    HMODULE _hModule = nullptr;

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
#endif // _LAGRANGE_CORE_NATIVEAPI_CPP_NATIVE_H_