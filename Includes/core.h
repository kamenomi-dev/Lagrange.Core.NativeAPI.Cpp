#pragma once

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_
#pragma region CORE

#include "common.h"
#include "wrapper.h"

#include <Windows.h>
#include <string>
#include <exception>

namespace LagrangeCore {

std::unique_ptr<DllExportsImpl> DllExports{};

void Initialize() {
    DllExports.reset(new DllExportsImpl);
};
void UnInitialize() {
    DllExports.release();
};

class Bot {
  public:
    Bot(
        BotConfigStruct bot
    ) {
        _currentIndex = DllExports->Initialize(&bot);
    };

    StatusCode Startup() { return _lastStatus = DllExports->Start(_currentIndex); };
    StatusCode Shutdown() { return _lastStatus = DllExports->Stop(_currentIndex); }

  private:
    StatusCode   _lastStatus{StatusCode::Success};
    ContextIndex _currentIndex{0};
};

} // namespace LagrangeCore

#pragma endregion CORE
#endif // _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_