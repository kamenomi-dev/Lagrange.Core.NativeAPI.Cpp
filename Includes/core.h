#pragma once
#pragma region CORE

// Todo List:
// 1. 实现类似 EventEmitter 的事件分发，并呈现上下文 Context   [   ]
// 2. 把 KeyStore 的操作解耦到 KeystoreController 中            [WIP]
// 3. 应该可以实现 Group, Private & Stranger 类                 [   ]

#include "logger.cpp"

#include "native_model.h"
#include "wrapper.h"
#include "keystore_controller.h"

#include "submodule/nlohmann/json.hpp"

#include <Windows.h>

#include <string>        // std::string, std::wstring
#include <filesystem>    // std::filesystem
#include <format>        // std::format
#include <mutex>         // std::mutex
#include <thread>        // std::thread
#include <atomic>        // std::atomic_bool
#include <unordered_map> // std::unordered_map


using Json = nlohmann::json;

namespace fs = std::filesystem;

namespace LagrangeCore {

class Bot {

    enum LoginTypes {
        Invalid,
        QuickLogin,
        QrCodeLogin
    };

  public:
    Bot(
        int64_t uin, NativeModel::Common::BotConfig config
    )
    : _keystoreController(uin) {
        _contextIndex = DllExports->Initialize(&config, _keystoreController.Get());
        _keystoreController.BindContext(_contextIndex);
    };

    auto Login() { return _lastStatus = DllExports->Start(_contextIndex); };
    auto Logout() { return _lastStatus = DllExports->Stop(_contextIndex); }

    void PollingEventThread() {
        ListEventCount();
        HandleQRCodeEvent();
        _keystoreController.Poll();
    }

    void HandleLogEvent() {
        auto result = (NativeModel::Event::EventArray*)DllExports->GetBotLogEvent(_contextIndex);
        if (result == nullptr) {
            return;
        }

        for (auto idx = 0; idx < result->count; idx++) {
            INT_PTR pCurrEvent = result->events + idx * sizeof(NativeModel::Event::BotLogEvent);
            auto*   currEvent  = (NativeModel::Event::BotLogEvent*)pCurrEvent;

            spdlog::info(currEvent->Message.ToString());
        }

        DllExports->FreeMemory((INT_PTR)result);
    }

    void HandleQRCodeEvent() {
        auto result = (NativeModel::Event::EventArray*)DllExports->GetQrCodeEvent(_contextIndex);
        if (result == nullptr) {
            return;
        }

        for (auto idx = 0; idx < result->count; idx++) {
            INT_PTR pCurrEvent = result->events + idx * sizeof(NativeModel::Event::BotQrCodeEvent);
            auto*   currEvent  = (NativeModel::Event::BotQrCodeEvent*)pCurrEvent;

            spdlog::info("Login via QRCode, url: {}", currEvent->Url.ToString());
            std::ofstream("qrcode.png", std::ios::binary) << currEvent->Image.ToString();
        }

        DllExports->FreeMemory((INT_PTR)result);
    }

    void ListEventCount() {
        auto eventCounts = (NativeModel::Event::ReverseEventCount*)DllExports->GetEventCount(_contextIndex);
        if (eventCounts == nullptr) {
            return;
        }

        static NativeModel::Event::ReverseEventCount lastEventCounts{};
        if (lastEventCounts == *eventCounts) {
            // No new events.
            DllExports->FreeMemory((INT_PTR)eventCounts);
            return;
        }

        lastEventCounts = *eventCounts;

        // Login part:
        spdlog::info(
            "[Event Count - Login] Online:{} | Login:{}", eventCounts->BotOnlineEventCount,
            eventCounts->BotLoginEventCount
        );
        spdlog::info(
            "[Event Count - Login] SMS: {} | Captcha:{} | NewDeviceVerify:{} | QrCode:{} | QrCodeQuery:{} | "
            "RefreshKeyStore:{}",
            eventCounts->BotSMSEventCount, eventCounts->BotCaptchaEventCount, eventCounts->BotNewDeviceVerifyEventCount,
            eventCounts->BotQrCodeEventCount, eventCounts->BotQrCodeQueryEventCount,
            eventCounts->BotRefreshKeystoreEventCount
        );

        // Others:
        spdlog::info(
            "[Event Count - Others] Log:{} | Message:{}", eventCounts->BotLogEventCount,
            eventCounts->BotMessageEventCount
        );

        DllExports->FreeMemory((INT_PTR)eventCounts);
    }

  private:
    // BotRawKeystore _LoadDeviceFile(
    //     const fs::path deviceFile
    //) {
    //     BotRawKeystore out{};
    //     _loginType = LoginTypes::QrCodeLogin;

    //    if (fs::exists(deviceFile)) {
    //        std::ifstream ifs{deviceFile};
    //        if (!ifs) {
    //            spdlog::error(
    //                L"Couldn't load device file {}. Is it existant? ", deviceFile.filename().generic_wstring()
    //            );
    //            return out;
    //        }

    //        Json json;
    //        try {
    //            ifs >> json;
    //            json.get_to(out);
    //            _loginType = LoginTypes::QuickLogin;
    //            return out;
    //        } catch (const std::exception& e) {
    //            spdlog::error("Failed to parse device file: {}, reason: {}", deviceFile.string(), e.what());
    //        }
    //    }

    //    return out;
    //}

    // void _SaveDeviceFile(
    //     const fs::path deviceFile, const BotRawKeystore& keystore
    //) {
    //     std::ofstream ofs{deviceFile};
    //     if (!ofs) {
    //         spdlog::error(L"Couldn't save device file {}. Is it writable? ",
    //         deviceFile.filename().generic_wstring()); return;
    //     }

    //    ofs << Json(keystore).dump(4);
    //    ofs.close();
    //}

    // NativeModel::Common::BotKeystore _TranslateKeystoreFromRaw(
    //     const BotRawKeystore& raw
    //) {
    //     NativeModel::Common::BotKeystore ret{};
    //     ret.Uin = raw.Uin;

    //    auto toByteArray = [](const std::string& str) -> NativeModel::Common::ByteArrayNative {
    //        NativeModel::Common::ByteArrayNative arr;
    //        arr.Length = static_cast<UINT>(str.size());
    //        arr.Data   = reinterpret_cast<INT_PTR>(str.c_str());
    //        return arr;
    //    };

    //    ret.Uid        = toByteArray(raw.Uid);
    //    ret.Guid       = toByteArray(raw.Guid);
    //    ret.AndroidId  = toByteArray(raw.AndroidId);
    //    ret.Qimei      = toByteArray(raw.Qimei);
    //    ret.DeviceName = toByteArray(raw.DeviceName);

    //    // WLoginSigs
    //    ret.A2                 = toByteArray(raw.WLoginSigs.A2);
    //    ret.A2Key              = toByteArray(raw.WLoginSigs.A2Key);
    //    ret.D2                 = toByteArray(raw.WLoginSigs.D2);
    //    ret.D2Key              = toByteArray(raw.WLoginSigs.D2Key);
    //    ret.A1                 = toByteArray(raw.WLoginSigs.A1);
    //    ret.A1Key              = toByteArray(raw.WLoginSigs.A1Key);
    //    ret.NoPicSig           = toByteArray(raw.WLoginSigs.NoPicSig);
    //    ret.TgtgtKey           = toByteArray(raw.WLoginSigs.TgtgtKey);
    //    ret.StKey              = toByteArray(raw.WLoginSigs.StKey);
    //    ret.StWeb              = toByteArray(raw.WLoginSigs.StWeb);
    //    ret.St                 = toByteArray(raw.WLoginSigs.St);
    //    ret.WtSessionTicket    = toByteArray(raw.WLoginSigs.WtSessionTicket);
    //    ret.WtSessionTicketKey = toByteArray(raw.WLoginSigs.WtSessionTicketKey);
    //    ret.RandomKey          = toByteArray(raw.WLoginSigs.RandomKey);
    //    ret.SKey               = toByteArray(raw.WLoginSigs.SKey);

    //    NativeModel::Common::ByteArrayDictNative arr{};
    //    ret.PsKey.Length = static_cast<UINT>(raw.WLoginSigs.PsKey.size());
    //    ret.PsKey.Data   = reinterpret_cast<INT_PTR>(raw.WLoginSigs.PsKey.c_str());

    //    return ret;
    //}

  private:
    LoginTypes         _loginType{LoginTypes::Invalid};
    fs::path           _deviceFile;
    StatusCode         _lastStatus{StatusCode::Success};
    ContextIndex       _contextIndex{0};
    KeystoreController _keystoreController;
};

class BotProcessor {
  public:
    static BotProcessor& Instance() {
        static BotProcessor instance{};
        return instance;
    }

    static volatile std::atomic_bool IsExit;

  public:
    void AddBot(
        _In_ Bot* botInstance
    ) {
        _botInstances.push_back(botInstance);
    }

    void Execute() {
        // Gracefully shutdown after pressing Control + C.
        std::signal(SIGINT, [](int signal) {
            IsExit = true;
            spdlog::info("Handled Exit Signal, waiting... ");
        });

        std::atomic_bool stopSignal{false};
        std::thread      pollingThread(
            [this](std::atomic_bool& stopSignal, std::vector<Bot*> bots) {
                std::lock_guard<std::mutex> lock{_pollingThreadMutex};
                spdlog::info("[Polling Thread] Thread started.");

                while (!stopSignal.load()) {
                    std::for_each(_botInstances.begin(), _botInstances.end(), [](Bot* bot) {
                        bot->PollingEventThread();
                    });

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                spdlog::info("[Polling Thread] Shut downing all bots.");

                std::for_each(_botInstances.begin(), _botInstances.end(), [](Bot* bot) { bot->Logout(); });

                spdlog::info("[Polling Thread] Received Exit-Signal and Exited.");
            },
            std::ref(stopSignal), std::ref(_botInstances)
        );

        std::for_each(_botInstances.begin(), _botInstances.end(), [](Bot* bot) { bot->Login(); });

        while (!IsExit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        spdlog::info("Processor's Thread exited.");
        stopSignal.store(true);
        pollingThread.join();
    }

  private:
    BotProcessor() = default;

    std::mutex        _pollingThreadMutex{};
    std::vector<Bot*> _botInstances{};
};

volatile std::atomic_bool BotProcessor::IsExit{false};

void Initialize() {
    Logger::InitializeLogger();

    // Here is not really necessary to check if DllExports has been initialized.
    // if (DllExports && DllExports->IsLoaded()) {
    //     spdlog::warn("DllExports has been already initialized. Check your code! ");
    //     return;
    // }

    DllExports = std::make_unique<DllExportsImpl>();
    BotProcessor::Instance();
};

void UnInitialize() {
    DllExports.release();
    Logger::UnInitializeLogger();
};

} // namespace LagrangeCore

#pragma endregion CORE