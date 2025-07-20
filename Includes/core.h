#pragma once

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_
#pragma region CORE

#include "defines.h"
#include "wrapper.h"

#include <Windows.h>
#include <string>
#include <exception>
#include <filesystem>
#include <thread>
#include <future>
#include <unordered_map>

#include "third_party/nlohmann/json.hpp"

using Json = nlohmann::json;

namespace fs = std::filesystem;

namespace LagrangeCore {

std::unique_ptr<DllExportsImpl> DllExports{};

class Bot {

    enum LoginTypes {
        Invalid,
        QuickLogin,
        QrCodeLogin
    };

  public:
    Bot(
        NativeTypes::BotConfigStruct config, fs::path deviceFile = ""
    ) {
        if (deviceFile.empty()) {
            deviceFile = "./temporary.keystore";
        }
        deviceFile.lexically_normal();
        _deviceFile = deviceFile;


        auto& keystoreRaw = _LoadDeviceFile(deviceFile);
        auto  keystore    = _TranslateKeystoreFromRaw(keystoreRaw);

        if (_loginType == QuickLogin) {
            _currentIndex = DllExports->Initialize(&config, &keystore);
            spdlog::info("Using Quick Login with device file: {}", deviceFile.string());
        } else {
            _currentIndex = DllExports->Initialize(&config);
            spdlog::info("Using QR Code Login with device file: {}", deviceFile.string());
        }
    };

    auto                    Startup() { return _lastStatus = DllExports->Start(_currentIndex); };
    NativeTypes::StatusCode Shutdown() { return _lastStatus = DllExports->Stop(_currentIndex); }

    void PollingEventThread(
        std::future<void> terminationSignal
    ) {
        while (terminationSignal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
            spdlog::info("114514");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

  private:
    CoreTypes::BotKeystoreStructRaw& _LoadDeviceFile(
        const fs::path deviceFile
    ) {
        CoreTypes::BotKeystoreStructRaw out{};
        _loginType = LoginTypes::QrCodeLogin;

        if (fs::exists(deviceFile)) {
            std::ifstream ifs{deviceFile};
            if (!ifs) {
                spdlog::error(
                    L"Couldn't load device file {}. Is it existant? ", deviceFile.filename().generic_wstring()
                );
                return out;
            }

            Json json;
            try {
                ifs >> json;
                LagrangeCore::CoreTypes::from_json(json, out);
                _loginType = LoginTypes::QuickLogin;
                return out;
            } catch (const std::exception& e) {
                spdlog::error("Failed to parse device file: {}, reason: {}", deviceFile.string(), e.what());
            }
        }

        return out;
    }

    void _SaveDeviceFile(
        const fs::path deviceFile, const CoreTypes::BotKeystoreStructRaw& keystore
    ) {
        Json json;
        LagrangeCore::CoreTypes::to_json(json, keystore);
        std::ofstream ofs{deviceFile};
        if (!ofs) {
            spdlog::error(L"Couldn't save device file {}. Is it writable? ", deviceFile.filename().generic_wstring());
            return;
        }
        ofs << json.dump(4);
        ofs.close();
    }

    NativeTypes::BotKeystoreStruct _TranslateKeystoreFromRaw(
        const CoreTypes::BotKeystoreStructRaw& raw
    ) {
        NativeTypes::BotKeystoreStruct ret{};
        ret.Uin = raw.Uin;

        auto toByteArray = [](const std::string& str) -> NativeTypes::ByteArray {
            NativeTypes::ByteArray arr;
            arr.Length = static_cast<UINT>(str.size());
            arr.Data   = reinterpret_cast<INT_PTR>(str.c_str());
            return arr;
        };

        ret.Uid        = toByteArray(raw.Uid);
        ret.Guid       = toByteArray(raw.Guid);
        ret.AndroidId  = toByteArray(raw.AndroidId);
        ret.Qimei      = toByteArray(raw.Qimei);
        ret.DeviceName = toByteArray(raw.DeviceName);

        // WLoginSigs
        ret.A2                 = toByteArray(raw.WLoginSigs.A2);
        ret.A2Key              = toByteArray(raw.WLoginSigs.A2Key);
        ret.D2                 = toByteArray(raw.WLoginSigs.D2);
        ret.D2Key              = toByteArray(raw.WLoginSigs.D2Key);
        ret.A1                 = toByteArray(raw.WLoginSigs.A1);
        ret.A1Key              = toByteArray(raw.WLoginSigs.A1Key);
        ret.NoPicSig           = toByteArray(raw.WLoginSigs.NoPicSig);
        ret.TgtgtKey           = toByteArray(raw.WLoginSigs.TgtgtKey);
        ret.StKey              = toByteArray(raw.WLoginSigs.StKey);
        ret.StWeb              = toByteArray(raw.WLoginSigs.StWeb);
        ret.St                 = toByteArray(raw.WLoginSigs.St);
        ret.WtSessionTicket    = toByteArray(raw.WLoginSigs.WtSessionTicket);
        ret.WtSessionTicketKey = toByteArray(raw.WLoginSigs.WtSessionTicketKey);
        ret.RandomKey          = toByteArray(raw.WLoginSigs.RandomKey);
        ret.SKey               = toByteArray(raw.WLoginSigs.SKey);
        ret.PsKey              = toByteArray(raw.WLoginSigs.PsKey);
        return ret;
    }

  private:
    LoginTypes                _loginType{LoginTypes::Invalid};
    fs::path                  _deviceFile;
    NativeTypes::StatusCode   _lastStatus{NativeTypes::StatusCode::Success};
    NativeTypes::ContextIndex _currentIndex{0};
};

class BotManager {
  public:
    static BotManager& Instance() {
        static BotManager instance{};
        return instance;
    }

    static volatile sig_atomic_t IsExit;

  public:
    void RegisterBot(
        _In_ Bot* botInstance
    ) {
        _bots.push_back(botInstance);
    }

    void Exec() {
        /*std::atexit([]() {
            IsExit = SIGINT;
            spdlog::info("Handled Exit Signal, waiting... ");
        });*/

        SetConsoleCtrlHandler(
            [](DWORD dwCtrlType) -> int {
                IsExit = SIGINT;
                spdlog::info("Handled Exit Signal, waiting... ");

            return TRUE;
        }, TRUE);

        std::atomic<bool> stopSignal{false};
        std::thread       pollingThread(
            [this](std::atomic<bool>& stopSignal) {
                spdlog::info("[Polling Thread] Thread started.");

                while (!stopSignal.load()) {
                    spdlog::info("[Polling Thread] Thread running.");
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                spdlog::info("[Polling Thread] Thread exited.");
            },
            std::ref(stopSignal)
        );

        while (IsExit == NULL) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        spdlog::info("Handled Exit signal.");
        stopSignal.store(true);
        pollingThread.join();
    }

  private:
    BotManager() = default;

    std::vector<Bot*> _bots{};
};

void Initialize() {
    Logger::InitializeLogger();
    if (DllExports && DllExports->IsLoaded()) {
        throw std::runtime_error("DllExports is already initialized.");
    }

    DllExports = std::make_unique<DllExportsImpl>();
    BotManager::Instance();
};

void UnInitialize() {
    DllExports.release();
    Logger::UnInitializeLogger();
};

volatile sig_atomic_t BotManager::IsExit{NULL};

} // namespace LagrangeCore

#pragma endregion CORE
#endif // _LAGRANGE_CORE_NATIVEAPI_CPP_CORE_H_