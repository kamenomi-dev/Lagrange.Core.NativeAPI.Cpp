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

ContextIndex DllExportsImpl::Initialize(
    BotConfigStruct* config, BotKeystoreStruct* keystore
) {
    if (!_InitializeFunc) {
        abort();
    }

    return _InitializeFunc(config, keystore);
}

StatusCode DllExportsImpl::Start(
    ContextIndex index
) {
    if (!_StartFunc) {
        abort();
    }

    return _StartFunc(index);
}

StatusCode DllExportsImpl::Stop(
    ContextIndex index
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
    ContextIndex index
) {
    if (!_GetEventCountFunc) {
        abort();
    }

    return _GetEventCountFunc(index);
}

INT_PTR DllExportsImpl::GetQrCodeEvent(
    ContextIndex index
) {
    if (!_GetQrCodeEventFunc) {
        abort();
    }

    return _GetQrCodeEventFunc(index);
}

INT_PTR DllExportsImpl::GetBotLogEvent(
    ContextIndex index
) {
    if (!_GetBotLogEventFunc) {
        abort();
    }

    return _GetBotLogEventFunc(index);
}
