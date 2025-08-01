#pragma once
#pragma region NativeApiWrapper

#define IMPORT_GETEVENT_INTERFACE(api)                                                                                 \
  public:                                                                                                              \
    INT_PTR api(_In_ ContextIndex index) {                                                                             \
        using Fn##api = INT_PTR(__cdecl*)(_In_ ContextIndex);                                                          \
                                                                                                                       \
        static auto fn##api    = LoadMethod<Fn##api>(#api);                                                            \
        static auto lastModule = _hModule;                                                                             \
                                                                                                                       \
        if (lastModule != _hModule) {                                                                                  \
            fn##api    = LoadMethod<Fn##api>(#api);                                                                    \
            lastModule = _hModule;                                                                                     \
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
    void UnloadNativeAPI();
    bool IsLoaded() const { return _hModule != nullptr; }

    // No Copy.
    DllExportsImpl(const DllExportsImpl&)            = delete;
    DllExportsImpl& operator=(const DllExportsImpl&) = delete;

    ~DllExportsImpl();

  public: // ------------------------- APIs ----------------------
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

    ContextIndex Initialize(
        _In_ NativeModel::Common::BotConfig* config, _In_ NativeModel::Common::BotKeystore* keystore = nullptr
    ) {
        using FnInitialize =
            ContextIndex(__cdecl*)(_In_ NativeModel::Common::BotConfig*, _In_ NativeModel::Common::BotKeystore*);

        static auto fnInitialize = LoadMethod<FnInitialize>("Initialize");
        static auto lastModule   = _hModule;

        if (lastModule != _hModule) {
            fnInitialize = LoadMethod<FnInitialize>("Initialize");
            lastModule   = _hModule;
        }

        if (!fnInitialize) {
            abort();
        }

        return fnInitialize(config, keystore);
    }

    StatusCode Start(
        _In_ ContextIndex index
    ) {
        using FnStart = StatusCode(__cdecl*)(_In_ ContextIndex);

        static auto fnStart    = LoadMethod<FnStart>("Start");
        static auto lastModule = _hModule;

        if (lastModule != _hModule) {
            fnStart    = LoadMethod<FnStart>("Start");
            lastModule = _hModule;
        }

        if (!fnStart) {
            abort();
        }

        return fnStart(index);
    }

    StatusCode Stop(
        _In_ ContextIndex index
    ) {
        using FnStop = StatusCode(__cdecl*)(_In_ ContextIndex);

        static auto fnStop     = LoadMethod<FnStop>("Stop");
        static auto lastModule = _hModule;

        if (lastModule != _hModule) {
            fnStop     = LoadMethod<FnStop>("Stop");
            lastModule = _hModule;
        }

        if (!fnStop) {
            abort();
        }

        return fnStop(index);
    }

    void FreeMemory(
        _In_ INT_PTR ptr
    ) {
        using FnFreeMemory = void(__cdecl*)(_In_ INT_PTR);

        static auto fnFreeMemory = LoadMethod<FnFreeMemory>("FreeMemory");
        static auto lastModule   = _hModule;

        if (lastModule != _hModule) {
            fnFreeMemory = LoadMethod<FnFreeMemory>("FreeMemory");
            lastModule   = _hModule;
        }

        if (!fnFreeMemory) {
            abort();
        }

        return fnFreeMemory(ptr);
    }

#pragma region Register Action API

    INT CreateMessageBuilder(
        _In_ ContextIndex index
    ) {
        using FnCreateMessageBuilder       = INT(__cdecl*)(_In_ ContextIndex);
        static auto fnCreateMessageBuilder = LoadMethod<FnCreateMessageBuilder>("CreateMessageBuilder");
        static auto lastModule             = _hModule;

        if (lastModule != _hModule) {
            fnCreateMessageBuilder = LoadMethod<FnCreateMessageBuilder>("CreateMessageBuilder");
            lastModule             = _hModule;
        }

        if (!fnCreateMessageBuilder) {
            abort();
        }
        return fnCreateMessageBuilder(index);
    }

    void AddText(_In_ ContextIndex index, _In_ INT builderIndex, _In_ NativeModel::Common::ByteArrayNative& text) {
        using FnAddText = void(__cdecl*)(_In_ ContextIndex, _In_ INT, _In_ NativeModel::Common::ByteArrayNative);
        static auto fnAddText = LoadMethod<FnAddText>("AddText");
        static auto lastModule = _hModule;

        if (lastModule != _hModule) {
            fnAddText = LoadMethod<FnAddText>("AddText");
            lastModule = _hModule;
        }

        if (!fnAddText) {
            abort();
        }

        return fnAddText(index, builderIndex, text);
    }

    INT SendGroupMessage(
        _In_ ContextIndex index, _In_ INT builderIndex, _In_ int64_t groupUin
    ) {
        using FnSendGroupMessage       = INT(__cdecl*)(_In_ ContextIndex, _In_ INT, _In_ int64_t);
        static auto fnSendGroupMessage = LoadMethod<FnSendGroupMessage>("SendGroupMessage");
        static auto lastModule         = _hModule;

        if (lastModule != _hModule) {
            fnSendGroupMessage = LoadMethod<FnSendGroupMessage>("SendGroupMessage");
            lastModule         = _hModule;
        }

        if (!fnSendGroupMessage) {
            abort();
        }

        return fnSendGroupMessage(index, builderIndex, groupUin);
    }

#pragma endregion Register Action API

#pragma region Register Event API
    IMPORT_GETEVENT_INTERFACE(GetEventCount);

    // Message
    IMPORT_GETEVENT_INTERFACE(GetMessageEvent);

    // Login
    IMPORT_GETEVENT_INTERFACE(GetQrCodeEvent);
    IMPORT_GETEVENT_INTERFACE(GetBotLogEvent);
    IMPORT_GETEVENT_INTERFACE(GetRefreshKeystoreEvent);
    IMPORT_GETEVENT_INTERFACE(GetNewDeviceVerifyEvent);
#pragma endregion Register Event API

    HMODULE      _hModule = nullptr;
    std::wstring _dllPath = L"Lagrange.Core.NativeAPI.dll";
};

extern std::unique_ptr<DllExportsImpl>& DllExports;

} // namespace LagrangeCore

#undef IMPORT_GETEVENT_INTERFACE
#pragma endregion NativeApiWrapper