#pragma once
#include <mutex>
#include <thread>
#include <vector>
#include <csignal>

#include "Core.h"
// #include "EventHandler.h"

#include "SubModule/eventpp/eventdispatcher.h"

namespace Lagrange {
class Bot : public Core::EventHandler {
  public:
    Bot(
        LONG uin, Definition::NativeModel::Common::BotConfig config = {}
    )
    : _keystore(uin),
      _botContext(DllExports::Initialize(&config, _keystore)) {
        _keystore.BindBotContext(_botContext);
        ((Core::EventHandler*)this)->BindContext(_botContext);
    }

    int64_t GetUin() const { return _keystore.GetUin(); }

    StatusCode Login() const { return DllExports::Start(_botContext); }
    StatusCode Logout() const { return DllExports::Stop(_botContext); }

    void PollEvent() {
        _keystore.PollEvent();
        ((Core::EventHandler*)this)->PollEvent();
    }

  private:
    Core::Keystore _keystore;
    HCONTEXT       _botContext{NULL};
};

class BotRegistry {
  public:
    BotRegistry()                              = default;
    BotRegistry(const BotRegistry&)            = delete;
    BotRegistry(const BotRegistry&&)           = delete;
    BotRegistry& operator=(const BotRegistry&) = delete;

    static auto& GetInstance() {
        static BotRegistry instance{};
        return instance;
    }

  public:
    BotRegistry& Register(
        Bot& bot
    ) {
        _bots.push_back(&bot);
        return *this;
    }

    void Run() {
        std::signal(SIGINT, [](int) -> void { _caughtExitSignal = true; });

        for (auto* bot : _bots) {
            bot->Login();
        }
        Logger::info("{} Bot(s) login.", _bots.size());

        while (!_caughtExitSignal) {
            for (auto* bot : _bots) {
                try {
                    bot->PollEvent();
                } catch (const std::exception& error) {
                    Logger::error("Bot({}) throws an exception as polling event. ", bot->GetUin());
                    Logger::error(error.what());
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(DelayTime));
        }

        for (auto* bot : _bots) {
            bot->Logout();
        }
        Logger::info("All bot(s) logout.");
    }

    void RunAsThread() {
        std::mutex  mtx{};
        std::lock_guard<std::mutex> lock{mtx};

        std::thread runningThread{[this]() -> void { Run(); }};
        if (runningThread.joinable()) {
            runningThread.join();
        }
    }

  public:
    inline static uint64_t DelayTime = 100; // ms

  private:
    inline static bool _caughtExitSignal = false;
    std::vector<Bot*>  _bots{};
};

} // namespace Lagrange