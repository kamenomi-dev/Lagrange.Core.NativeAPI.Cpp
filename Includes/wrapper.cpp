#pragma once
#include "wrapper.h"

using namespace LagrangeCore;

DllExportsImpl::DllExportsImpl(
    const std::wstring& dllPath
)
: _dllPath(dllPath) {
    LoadNativeAPI();
}

void DllExportsImpl::LoadNativeAPI() {
    if (_hModule) {
        spdlog::warn("Library has been already loaded. ");
        return;
    }

    _hModule = LoadLib(_dllPath.c_str());

    if (!_hModule) {
#ifdef __linux__
        auto error = dlerror();
#endif
        spdlog::error(L"Cannot load a invalid library {}! ", _dllPath);
        abort();
    }
}

void DllExportsImpl::UnloadNativeAPI() {
    if (!_hModule) {
        spdlog::warn("Library has not been loaded. ");
        return;
    }

    if (!FreeLib(_hModule)) {
        spdlog::error(L"Failed to unload library {}! ", _dllPath);
        abort();
    }

    _hModule = nullptr;
}

DllExportsImpl::~DllExportsImpl() {
    if (_hModule) {
        FreeLib(_hModule);
        _hModule = nullptr;
    }
}

std::unique_ptr<DllExportsImpl>& LagrangeCore::DllExports = DllExportsImpl::Instance();