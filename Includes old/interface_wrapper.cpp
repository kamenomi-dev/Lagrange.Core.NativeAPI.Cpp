#pragma once
#include "interface_wrapper.h"

using namespace LagrangeCore;

DllExportsImpl::DllExportsImpl(
    const std::wstring& libraryFilePath
)
: _libraryFilePath(libraryFilePath) {
    LoadDll();
}

void DllExportsImpl::LoadDll() {
    if (_hModule) {
        spdlog::warn("Library has been already loaded. ");
        return;
    }

    _hModule = LoadLib(_libraryFilePath.c_str());

    if (!_hModule) {
#ifdef __linux__
        auto error = dlerror();
#endif
        spdlog::error(L"Cannot load a invalid library {}! ", _libraryFilePath);
        abort();
    }
}

void DllExportsImpl::UnloadDll() {
    if (!_hModule) {
        spdlog::warn("Library has not been loaded yet. ");
        return;
    }

    if (!FreeLib(_hModule)) {
        spdlog::error(L"Failed to unload library {}! ", _libraryFilePath);
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