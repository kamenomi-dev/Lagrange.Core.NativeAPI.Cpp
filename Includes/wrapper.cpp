#include "wrapper.h"
using namespace LagrangeCore;

DllExportsImpl::DllExportsImpl(
    const std::wstring& dllPath
) {
    _hModule = LoadLibraryW(dllPath.c_str());
    if (!_hModule) {
        auto error = GetLastError();
        abort();
    }

    try {
        LoadMethod(_InitializeFunc, "Initialize");
        LoadMethod(_StartFunc, "Start");
        LoadMethod(_StopFunc, "Stop");
        LoadMethod(_FreeMemoryFunc, "FreeMemory");
        LoadMethod(_GetEventCountFunc, "GetEventCount");
        LoadMethod(_GetQrCodeEventFunc, "GetQrCodeEvent");
        LoadMethod(_GetBotLogEventFunc, "GetBotLogEvent");
    } catch (const std::exception& err) {
        throw err;
    }
}

DllExportsImpl::~DllExportsImpl() {
    if (_hModule) {
        FreeLibrary(_hModule);
        _hModule = nullptr;
    }
}

NativeTypes::ContextIndex DllExportsImpl::Initialize(
    NativeTypes::BotConfigStruct* config, NativeTypes::BotKeystoreStruct* keystore
) {
    if (!_InitializeFunc) {
        abort();
    }

    return _InitializeFunc(config, keystore);
}

NativeTypes::StatusCode DllExportsImpl::Start(
    NativeTypes::ContextIndex index
) {
    if (!_StartFunc) {
        abort();
    }

    return _StartFunc(index);
}

NativeTypes::StatusCode DllExportsImpl::Stop(
    NativeTypes::ContextIndex index
) {
    if (!_StopFunc) {
        abort();
    }

    return _StopFunc(index);
}

void DllExportsImpl::FreeMemory(
    INT_PTR ptr
) {
    if (!_FreeMemoryFunc) {
        abort();
    }

    return _FreeMemoryFunc(ptr);
}

INT_PTR DllExportsImpl::GetEventCount(
    NativeTypes::ContextIndex index
) {
    if (!_GetEventCountFunc) {
        abort();
    }

    return _GetEventCountFunc(index);
}

INT_PTR DllExportsImpl::GetQrCodeEvent(
    NativeTypes::ContextIndex index
) {
    if (!_GetQrCodeEventFunc) {
        abort();
    }

    return _GetQrCodeEventFunc(index);
}

INT_PTR DllExportsImpl::GetBotLogEvent(
    NativeTypes::ContextIndex index
) {
    if (!_GetBotLogEventFunc) {
        abort();
    }

    return _GetBotLogEventFunc(index);
}
