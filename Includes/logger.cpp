#pragma once
#include <string>

#pragma push_macro("FMT_UNICODE")
#pragma push_macro("SPDLOG_NO_EXCEPTIONS")
#pragma push_macro("SPDLOG_WCHAR_TO_UTF8_SUPPORT")

#define FMT_UNICODE 0
#define SPDLOG_NO_EXCEPTIONS
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#ifndef LAGRANGE_USING_SELF_LOGGER
#include "submodule/spdlog/spdlog.h"
#include "submodule/spdlog/sinks/stdout_sinks.h"
#include "submodule/spdlog/sinks/stdout_color_sinks.h"
#else
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#pragma pop_macro("FMT_UNICODE")
#pragma pop_macro("SPDLOG_NO_EXCEPTIONS")
#pragma pop_macro("SPDLOG_WCHAR_TO_UTF8_SUPPORT")

namespace LagrangeCore {
namespace Logger {

static void InitializeLogger() {
    static std::shared_ptr<spdlog::logger> logger;

    if (logger) {
        logger->warn("Logger has been already initialized. Check your code! ");
    }

    logger = spdlog::stdout_color_mt("LagrangeCore");
    spdlog::set_default_logger(logger);
    spdlog::set_error_handler([](const std::string& reason) {
        MessageBoxA(nullptr, reason.c_str(), "Handled a reported error! ", MB_OK);
        abort();
    });
}

static void UnInitializeLogger() {
    spdlog::shutdown();
}
} // namespace Logger
} // namespace LagrangeCore
