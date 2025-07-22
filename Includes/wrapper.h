#pragma once
#pragma region DllLoad

#define IMPORT_GETEVENT_INTERFACE(api)                                                                                 \
  public:                                                                                                              \
    using Fn##api = INT_PTR(__cdecl*)(_In_ ContextIndex);                                                              \
    INT_PTR api(_In_ ContextIndex index) {                                                                             \
        static auto fn##api    = LoadMethod<Fn##api>(#api);                                                            \
        static auto lastModule = _hModule;                                                                             \
                                                                                                                       \
        if (_hModule != lastModule) {                                                                                  \
            fn##api = LoadMethod<Fn##api>(#api);                                                                       \
        };                                                                                                             \
                                                                                                                       \
        if (!fn##api) {                                                                                                \
            abort();                                                                                                   \
        };                                                                                                             \
                                                                                                                       \
        return fn##api(index);                                                                                         \
    }

#include "logger.cpp"
#include "native_model.h"

#include <Windows.h>
#include <string>
#include <memory>
#include <exception>

namespace LagrangeCore {
class DllExportsImpl {
  public:
    static std::unique_ptr<DllExportsImpl>& Instance(
        const std::wstring& dllPath = L"Lagrange.Core.NativeAPI.dll"
    ) {
        static std::unique_ptr<DllExportsImpl> instance = std::make_unique<DllExportsImpl>(dllPath);
        return instance;
    }

    // Not initialize via this constructor directly.
    explicit DllExportsImpl(const std::wstring& dllPath = L"Lagrange.Core.NativeAPI.dll");
    void LoadNativeAPI();
    void UnloadLibrary();
    bool IsLoaded() const { return _hModule != nullptr; }

    // No Copy.
    DllExportsImpl(const DllExportsImpl&)            = delete;
    DllExportsImpl& operator=(const DllExportsImpl&) = delete;

    ~DllExportsImpl();

  public: // ------------------------- APIs ----------------------
    using FnInitialize =
        ContextIndex(__cdecl*)(_In_ NativeModel::Common::BotConfig*, _In_ NativeModel::Common::BotKeystore*);
    using FnStart      = StatusCode(__cdecl*)(_In_ ContextIndex);
    using FnStop       = StatusCode(__cdecl*)(_In_ ContextIndex);
    using FnFreeMemory = void(__cdecl*)(_In_ INT_PTR);

    // Actions
    using FnSendGroupMessage  = INT_PTR(__cdecl*)(_In_ ContextIndex);
    using FnSendFriendMessage = INT_PTR(__cdecl*)(_In_ ContextIndex);

    template <typename FuncType>
    FuncType LoadMethod(
        const char* funcName
    ) {
        auto funcPtr = (FuncType)GetProcAddress(_hModule, funcName);
        if (!funcPtr) {
            spdlog::error("Unknown interface. Is that using an incompatible library? ");
            return nullptr;
        }

        return funcPtr;
    }

    ContextIndex Initialize(_In_ NativeModel::Common::BotConfig*, _In_ NativeModel::Common::BotKeystore* = nullptr);
    StatusCode   Start(_In_ ContextIndex);
    StatusCode   Stop(_In_ ContextIndex);
    void         FreeMemory(_In_ INT_PTR);

    IMPORT_GETEVENT_INTERFACE(GetEventCount);
    IMPORT_GETEVENT_INTERFACE(GetQrCodeEvent);
    IMPORT_GETEVENT_INTERFACE(GetBotLogEvent);
    IMPORT_GETEVENT_INTERFACE(GetRefreshKeystoreEvent);
    IMPORT_GETEVENT_INTERFACE(GetNewDeviceVerifyEvent);

    HMODULE      _hModule = nullptr;
    std::wstring _dllPath = L"Lagrange.Core.NativeAPI.dll";

    FnInitialize _InitializeFunc = nullptr;
    FnStart      _StartFunc      = nullptr;
    FnStop       _StopFunc       = nullptr;
    FnFreeMemory _FreeMemoryFunc = nullptr;
};

extern std::unique_ptr<DllExportsImpl>& DllExports;

} // namespace LagrangeCore

#undef IMPORT_GETEVENT_INTERFACE
#pragma endregion DllLoad