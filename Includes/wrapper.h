#pragma once

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_NATIVE_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_NATIVE_H_
#pragma region DllLoad

#include "Defines.h"

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
    typedef NativeTypes::ContextIndex(__cdecl* FnInitialize)(_In_ NativeTypes::BotConfigStruct*, _In_ NativeTypes::BotKeystoreStruct*);
    typedef NativeTypes::StatusCode(__cdecl* FnStart)(NativeTypes::ContextIndex);
    typedef NativeTypes::StatusCode(__cdecl* FnStop)(NativeTypes::ContextIndex);
    typedef void(__cdecl* FnFreeMemory)(INT_PTR);
    typedef INT_PTR(__cdecl* FnGetEventCount)(NativeTypes::ContextIndex);

    // Events
    typedef INT_PTR(__cdecl* FnGetLoginEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetOnlineEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetMessageEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetBotLogEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetSMSEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetQrCodeEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetQrCodeQueryEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetRefreshKeytoreEvent)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnGetNewDeviceVerifyEvent)(NativeTypes::ContextIndex);

    // Actions
    typedef INT_PTR(__cdecl* FnSengGroupMessage)(NativeTypes::ContextIndex);
    typedef INT_PTR(__cdecl* FnSendFriendMessage)(NativeTypes::ContextIndex);


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

    NativeTypes::ContextIndex Initialize(_In_ NativeTypes::BotConfigStruct*, _In_ NativeTypes::BotKeystoreStruct* = nullptr);
    NativeTypes::StatusCode   Start(_In_ NativeTypes::ContextIndex);
    NativeTypes::StatusCode   Stop(_In_ NativeTypes::ContextIndex);
    void                 FreeMemory(_In_ INT_PTR);
    INT_PTR              GetEventCount(_In_ NativeTypes::ContextIndex);
    INT_PTR              GetQrCodeEvent(_In_ NativeTypes::ContextIndex);
    INT_PTR              GetBotLogEvent(_In_ NativeTypes::ContextIndex);

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