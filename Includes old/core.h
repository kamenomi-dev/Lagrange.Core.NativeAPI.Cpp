#pragma once
#pragma region Lagrange Core

#include "logger.cpp"
#include "interface_wrapper.h"
#include "native_model.h"
#include "event_handler.h"
#include "keystore_controller.h"

#include "submodule/nlohmann/json.hpp"

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <csignal>
#include <unordered_map>
#include <vector>
#include <chrono>

using Json   = nlohmann::json;
namespace fs = std::filesystem;

namespace LagrangeCore {

class Bot : public Event::EventHandler {
  public:
    explicit Bot(
        int64_t uin, const NativeModel::Common::BotConfig& config
    )
    : _botUin(uin),
      _keystoreHandler(uin) {
        InitializeContext(config);
    }

    uint64_t GetUin() const noexcept { return _botUin; }

    auto Login() const { return DllExports->Start(_contextIndex); }
    auto Logout() const { return DllExports->Stop(_contextIndex); }

    void PollEvent() {
        _keystoreHandler.PollEvent();
        EventHandler::PollEvent();
    }

  private:
    void InitializeContext(
        const NativeModel::Common::BotConfig& config
    ) {
        _contextIndex = DllExports->Initialize(&config, _keystoreHandler.Get());
        EventHandler::Bind(_contextIndex);
        _keystoreHandler.Bind(_contextIndex);
    }

  private:
    uint64_t        _botUin{0};
    ContextIndex    _contextIndex{-1};
    KeystoreHandler _keystoreHandler;
};

class BotPool {
  public:
    static BotPool& Instance() {
        static BotPool instance{};
        return instance;
    }

    void EmplaceBot(
        Bot& botInstance
    ) {
        _bots.push_back(&botInstance);
    }

    void Execute() {
        SetupSignalHandler();
        StartPolling();

        for (auto& bot : _bots) bot->Login();
        spdlog::info("{} bot(s) login.", _bots.size());

        WaitExit();

        for (auto& bot : _bots) bot->Logout();
        spdlog::info("{} bot(s) logout.", _bots.size());

        StopPolling();
    }

  public:
    /// <summary>事件轮询延时，单位：毫秒。</summary>
    uint64_t PollThreadDelay{10};
    /// <summary>检测退出信号间隔，单位：毫秒。</summary>
    uint64_t AwaitExitPerDelay{1000};

  private:
    BotPool() = default;

    void SetupSignalHandler() {
        IsExit = false;
        std::signal(SIGINT, [](int) {
            IsExit = true;
            spdlog::info("[Polling Thread] Exit signal received, shutting down...");
        });
    }

    void StartPolling() {
        _threadTerminateSignal     = false;
        _thread = std::thread([this] {
            spdlog::info("[Polling Thread] Started.");
            while (!_threadTerminateSignal.load()) {
                for (auto& bot : _bots) {
                    try {
                        bot->PollEvent();
                    } catch (const std::exception& err) {
                        spdlog::error("[Polling Thread] Bot({}) error: {}", bot->GetUin(), err.what());
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(PollThreadDelay));
            }
            spdlog::info("[Polling Thread] Exited.");
        });
    }

    void StopPolling() {
        _threadTerminateSignal = true;
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void WaitExit() {
        while (!IsExit.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(AwaitExitPerDelay));
        }
    }

  private:
    std::vector<Bot*>              _bots;
    std::thread                    _thread;
    std::atomic_bool               _threadTerminateSignal{false};
    inline static std::atomic_bool IsExit{false};
};

void Initialize() {
    Logger::InitializeLogger();
    DllExports = std::make_unique<DllExportsImpl>();
    BotPool::Instance();
}

void UnInitialize() {
    DllExports.reset();
    Logger::UnInitializeLogger();
}

} // namespace LagrangeCore

#pragma endregion
