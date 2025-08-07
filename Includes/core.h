#pragma once
#pragma region Lagrange Core

// Todo List:
// 1. 实现类似 EventEmitter 的事件分发，并呈现上下文 Context   [WIP]
// 2. 把 KeyStore 的操作解耦到 KeystoreHandler 中               [OK ]
// 3. 应该可以实现 Group, Private & Stranger 类                 [WIP]

#include "logger.cpp"

#include "interface_wrapper.h"
#include "native_model.h"
#include "event_handler.h"
#include "keystore_controller.h"

#include "submodule/nlohmann/json.hpp"

#include <Windows.h>

#include <string>        // std::string, std::wstring
#include <format>        // std::format
#include <mutex>         // std::mutex
#include <thread>        // std::thread
#include <atomic>        // std::atomic_bool
#include <csignal>       // std::signal
#include <unordered_map> // std::unordered_map

using Json = nlohmann::json;

namespace fs = std::filesystem;

namespace LagrangeCore {

class Bot : public Event::EventHandler {
  public:
    Bot(
        int64_t uin, NativeModel::Common::BotConfig config
    )
    : _botUin(uin),
      _keystoreHandler(uin) {
        _contextIndex = DllExports->Initialize(&config, _keystoreHandler.Get());

        EventHandler::Bind(_contextIndex);
        _keystoreHandler.Bind(_contextIndex);
    }

    auto GetUin() const { return _botUin; }

    auto Login() const { return DllExports->Start(_contextIndex); }
    auto Logout() const { return DllExports->Stop(_contextIndex); }

    void PollEvent() {
        _keystoreHandler.PollEvent();
        EventHandler::PollEvent();
    }

  private:
    uint64_t        _botUin{UINT_MAX};
    ContextIndex    _contextIndex{INT_MIN};
    KeystoreHandler _keystoreHandler;
};

class BotPool {
  public:
    static BotPool& Instance() {
        static BotPool instance{};
        return instance;
    }

  public:
    void AddBot(
        _In_ Bot& botInstance
    ) {
        _bots.push_back(&botInstance);
    }

    void Execute() {
        // Gracefully shutdown after pressing Control-C.
        BotPool::IsExit = false;
        std::signal(SIGINT, [](int signal) {
            IsExit = true;
            spdlog::info("[Polling Thread] Handled Exit Signal, waiting... ");
        });

        std::atomic_bool stopSignal{false};
        std::thread      pollingThread(
            [this](std::atomic_bool& stopSignal) {
                std::lock_guard<std::mutex> lock{_threadMutex};
                spdlog::info("[Polling Thread] Running!.");

                while (!stopSignal.load()) {
                    for (auto& bot : _bots) try {
                            bot->PollEvent();
                        } catch (const std::exception& err) {
                            spdlog::error("[Polling Thread] Bot({}) thrown an error: \n{}", bot->GetUin(), err.what());
                        }

                    std::this_thread::sleep_for(std::chrono::milliseconds(PollThreadDelay));
                }
            },
            std::ref(stopSignal)
        );

        for (auto& bot : _bots) bot->Login();
        spdlog::info("{} bot(s) login.", _bots.size());

        while (!IsExit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(AwaitExitPerDelay));
        }

        for (auto& bot : _bots) bot->Logout();
        spdlog::info("{} bot(s) logout.", _bots.size());

        stopSignal.store(true);
        pollingThread.join();

        spdlog::info("[Polling Thread] Exited.");
    }

  public:
    /// <summary>Unit: millisecond.</summary>
    uint64_t PollThreadDelay{10};
    /// <summary>Unit: millisecond.</summary>
    uint64_t AwaitExitPerDelay{1000};

  private:
    BotPool() = default;
    std::mutex        _threadMutex{};
    std::vector<Bot*> _bots;

    static volatile std::atomic_bool IsExit;
};

volatile std::atomic_bool BotPool::IsExit{false};

void Initialize() {
    Logger::InitializeLogger();
    DllExports = std::make_unique<DllExportsImpl>();
    BotPool::Instance();
};

void UnInitialize() {
    DllExports.release();
    Logger::UnInitializeLogger();
};

} // namespace LagrangeCore

#pragma endregion