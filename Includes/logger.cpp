#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_LOGGER_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_LOGGER_H_

#include <string>

#define FMT_UNICODE 0
#define SPDLOG_NO_EXCEPTIONS
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

// 注意这里，我用的spdlog库在其他项目引用了，后续要改

#include "third_party/spdlog/spdlog.h"
#include "third_party/spdlog/sinks/stdout_sinks.h"
#include "third_party/spdlog/sinks/stdout_color_sinks.h"

namespace LagrangeCore {
namespace Logger {
static void _Private_ErrorHandler(
    const std::string& reason
) {
    MessageBoxA(nullptr, reason.c_str(), "Lagrange Core error! ", MB_OK);
    abort();
}
static void InitializeLogger() {
    static auto logger = spdlog::stdout_color_mt("LagrangeCore");
    spdlog::set_default_logger(logger);
    spdlog::set_error_handler(_Private_ErrorHandler);
}

static void UnInitializeLogger() {
    spdlog::shutdown();
}
} // namespace Logger
} // namespace LagrangeCore

#endif
