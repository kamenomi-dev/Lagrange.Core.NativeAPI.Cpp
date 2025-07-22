#pragma once
#include "wrapper.h"

using namespace LagrangeCore;

namespace LagrangeCore {
extern auto& DllExports = DllExportsImpl::Instance();
};

DllExportsImpl::DllExportsImpl(
    const std::wstring& dllPath
)
: _dllPath(dllPath) {
    LoadNativeAPI();

    try {
        _InitializeFunc = LoadMethod<FnInitialize>("Initialize");
        _StartFunc      = LoadMethod<FnStart>("Start");
        _StopFunc       = LoadMethod<FnStop>("Stop");
        _FreeMemoryFunc = LoadMethod<FnFreeMemory>("FreeMemory");
    } catch (const std::exception& err) {
        throw err;
    }
}

void DllExportsImpl::LoadNativeAPI() {
    if (_hModule) {
        spdlog::warn("Library has been already loaded. ");
        return;
    }

    _hModule = LoadLibraryW(_dllPath.c_str());

    if (!_hModule) {
        spdlog::error(L"Cannot load a invalid library {}! ", _dllPath);
        abort();
    }
}

void DllExportsImpl::UnloadLibrary() {
    if (!_hModule) {
        spdlog::warn("Library has not been loaded. ");
        return;
    }

    if (!FreeLibrary(_hModule)) {
        spdlog::error(L"Failed to unload library {}! ", _dllPath);
        abort();
    }

    _hModule = nullptr;
}

DllExportsImpl::~DllExportsImpl() {
    if (_hModule) {
        FreeLibrary(_hModule);
        _hModule = nullptr;
    }
}

ContextIndex DllExportsImpl::Initialize(
    _In_ NativeModel::Common::BotConfig* config, _In_ NativeModel::Common::BotKeystore* keystore
) {
    if (!_InitializeFunc) {
        abort();
    }

    return _InitializeFunc(config, keystore);
}

StatusCode DllExportsImpl::Start(
    _In_ ContextIndex index
) {
    if (!_StartFunc) {
        abort();
    }

    return _StartFunc(index);
}

StatusCode DllExportsImpl::Stop(
    _In_ ContextIndex index
) {
    if (!_StopFunc) {
        abort();
    }

    return _StopFunc(index);
}

void DllExportsImpl::FreeMemory(
    _In_ INT_PTR ptr
) {
    if (!_FreeMemoryFunc) {
        abort();
    }

    return _FreeMemoryFunc(ptr);
}
