#pragma once
#include <vector>
#include <csignal>

#include "Core.h"
#include "EventHandler.h"

namespace Lagrange {
class Bot {
  public:
    Bot(
        LONG uin, Definition::NativeModel::Common::BotConfig config = {}
    )
    : _keystore(uin) {
        _botContext = DllExports::Initialize(&config, _keystore);
        _keystore.BindBotContext(_botContext);
        _eventHandler.BindContext(_botContext);
    }

    LONG GetUin() const { return _keystore.GetUin(); }

    StatusCode Login() const { return DllExports::Start(_botContext); }
    StatusCode Logout() const { return DllExports::Stop(_botContext); }

    void PollEvent() {
        _keystore.PollEvent();
        _eventHandler.PollEvent();
    }

  private:
    HCONTEXT           _botContext{NULL};
    Core::Keystore     _keystore;
    Core::EventHandler _eventHandler;
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
                bot->PollEvent();
                try {
                    // bot->PollEvent();
                } catch (const std::exception& error) {
                    Logger::error("Bot({}) throws an exception as polling event. ", bot->GetUin());
                    Logger::error(error.what());
                }
            }
        }

        for (auto* bot : _bots) {
            bot->Logout();
        }
        Logger::info("All bot(s) logout.");
    }

  private:
    inline static bool _caughtExitSignal = false;
    std::vector<Bot*>  _bots{};
};

} // namespace Lagrange