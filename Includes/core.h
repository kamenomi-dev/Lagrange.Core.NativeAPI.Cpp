#pragma once
#pragma region Lagrange Core

// Todo List:
// 1. 实现类似 EventEmitter 的事件分发，并呈现上下文 Context   [WIP]
// 2. 把 KeyStore 的操作解耦到 KeystoreController 中            [OK ]
// 3. 应该可以实现 Group, Private & Stranger 类                 [WIP]

#include "logger.cpp"

#include "native_model.h"
#include "wrapper.h"
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

class Bot : public EventHandler::EventHandler {
  public:
    Bot(
        int64_t uin, NativeModel::Common::BotConfig config
    )
    : _uin(uin),
      _keystoreController(uin) {
        _contextIndex = DllExports->Initialize(&config, _keystoreController.Get());
        _keystoreController.BindContext(_contextIndex);

        EventHandler::BindContext(_contextIndex);
    }

    auto GetUin() const { return _uin; }

    auto Login() { return _lastStatus = DllExports->Start(_contextIndex); }
    auto Logout() { return _lastStatus = DllExports->Stop(_contextIndex); }

    void PollEvent() {
        EventHandler::PollEvent();
        _keystoreController.Poll();
    }

  private:
    uint64_t           _uin{UINT_MAX};
    StatusCode         _lastStatus{StatusCode::Success};
    ContextIndex       _contextIndex{INT_MIN};
    KeystoreController _keystoreController;
};

class BotProcessor {
  public:
    static BotProcessor& Instance() {
        static BotProcessor instance{};
        return instance;
    }

  public:
    void AddBot(
        _In_ Bot* botInstance
    ) {
        _bots.push_back(botInstance);
    }

    void Execute() {
        // Gracefully shutdown after pressing Control-C.
        BotProcessor::IsExit = false;
        std::signal(SIGINT, [](int signal) {
            IsExit = true;
            spdlog::info("[Polling Thread] Handled Exit Signal, waiting... ");
        });

        std::atomic_bool stopSignal{false};
        std::thread      pollingThread(
            [this](std::atomic_bool& stopSignal, std::vector<Bot*> bots) {
                std::lock_guard<std::mutex> lock{_threadMutex};
                spdlog::info("[Polling Thread] Thread started.");

                while (!stopSignal.load()) {
                    for (auto& bot : _bots) {
                        try {
                            bot->PollEvent();
                        } catch (const std::exception& err) {
                            spdlog::error(
                                "[Polling Thread] Polling events, a bot({}) thrown an error: \n{}",
                                bot->GetUin(),
                                err.what()
                            );
                        }
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(PollingThreadDelay));
                }

                spdlog::info("[Polling Thread] Shut downing all bots.");

                for (auto& bot : _bots) bot->Logout();
            },
            std::ref(stopSignal),
            std::ref(_bots)
        );

        for (auto& bot : _bots) bot->Login();

        while (!IsExit) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        stopSignal.store(true);
        pollingThread.join();

        spdlog::info("[Polling Thread] Thread exited.");
    }

  public:
    /// <summary>Unit: millisecond.</summary>
    uint64_t PollingThreadDelay{10};

  private:
    static volatile std::atomic_bool IsExit;

    BotProcessor() = default;
    std::mutex        _threadMutex{};
    std::vector<Bot*> _bots{};
};

volatile std::atomic_bool BotProcessor::IsExit{false};

void Initialize() {
    Logger::InitializeLogger();
    DllExports = std::make_unique<DllExportsImpl>();
    BotProcessor::Instance();
};

void UnInitialize() {
    DllExports.release();
    Logger::UnInitializeLogger();
};

} // namespace LagrangeCore

#pragma endregion