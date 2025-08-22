#pragma once
#include <iostream>
#include <filesystem>

#define FMT_UNICODE 0
#define SPDLOG_WCHAR_FILENAMES
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include "SubModule/spdlog/spdlog.h"
#include "SubModule/spdlog/sinks/stdout_color_sinks.h"
#include "SubModule/spdlog/sinks/basic_file_sink.h"

#include "Utils.h"

namespace FileSystem = std::filesystem;

namespace Lagrange::Logger {

extern const std::string LOGGER_BASE_NAME{"NativeAPI.C++"};

std::vector<spdlog::sink_ptr> _sinks;

extern void Initialize() {
    FileSystem::path logFile = FileSystem::absolute("./Logs/");
    FileSystem::create_directory(logFile);

    logFile = logFile.append(Utils::GetCurrentLocalTimeAsISO8601()).replace_extension(".log");

    try {
        const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        const auto file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.generic_wstring(), true);

        _sinks.push_back(console_sink);
        _sinks.push_back(file_sink);

        // 例如: [2023-10-27 10:30:00.123] [MyApp.Main] [info] Welcome!
        for (auto& sink : _sinks) {
            sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        }

        const auto mainLoggerName = LOGGER_BASE_NAME + ".Main";
        const auto mainLogger     = std::make_shared<spdlog::logger>(mainLoggerName, _sinks.begin(), _sinks.end());
        spdlog::register_logger(mainLogger);
        spdlog::set_default_logger(mainLogger);

#ifdef _DEBUG
        spdlog::set_level(spdlog::level::trace);
        mainLogger->set_level(spdlog::level::trace);
#endif

        std::printf("Logger Initialized \n");
    } catch (const spdlog::spdlog_ex& error) {
        std::cerr << "Logger Initialization Failed: " << error.what() << std::endl;
    }
}

std::shared_ptr<spdlog::logger> Get(
    const std::string& name
) {
    const auto loggerName = LOGGER_BASE_NAME + "." + name;
    auto       logger     = spdlog::get(loggerName);

    if (logger == nullptr) {
        logger = std::make_shared<spdlog::logger>(loggerName, _sinks.begin(), _sinks.end());
        logger->set_level(spdlog::get_level());
        spdlog::register_logger(logger);
    }

    return logger;
};

extern void Uninitialize() {
    spdlog::shutdown();
}

using spdlog::critical;
using spdlog::debug;
using spdlog::error;
using spdlog::info;
using spdlog::trace;
using spdlog::warn;
} // namespace Lagrange::Logger